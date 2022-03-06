#include "planner.h"

#include "../global.h"

#include <absl/strings/str_format.h>
#include <ortools/sat/cp_model.h>
#include <ortools/sat/model.h>
#include <ortools/sat/sat_parameters.pb.h>
#include <ortools/util/time_limit.h>

#include <QDebug>

bool available(const QString& availabitilty, const bool morning)
{
    if (availabitilty == G::DayShift || availabitilty == G::MorningOrAfternoonShift)
    {
        return true;
    }

    if (availabitilty == G::MorningShift && morning)
    {
        return true;
    }

    if (availabitilty == G::AfternoonShift && !morning)
    {
        return true;
    }

    return false;
}

Planner::Planner(QObject* parent) : QObject(parent)
{
    //
}


// Nurse scheduling problem with shift requests.
#include <atomic>
#include <numeric>
#include <vector>

#include "absl/strings/str_format.h"
#include "ortools/sat/cp_model.h"
#include "ortools/sat/model.h"
#include "ortools/sat/sat_parameters.pb.h"

using namespace operations_research::sat;

void Planner::ScheduleRequestsSat()
{
    const QStringList& dates = m_availabilityModel->dates();

    //! data definition
    const int num_workers       = m_skillHourModel->rowCount();
    const int num_shifts        = G::ShiftsCount;
    const int num_slots_per_day = G::SlotsPerDay;
    const int num_days          = dates.size();
    const int num_slots         = num_days * num_slots_per_day;

    std::vector<int> all_workers(num_workers);
    std::iota(all_workers.begin(), all_workers.end(), 0);

    std::vector<int> all_shifts(num_shifts);
    std::iota(all_shifts.begin(), all_shifts.end(), 0);

    std::vector<int> all_slots(num_slots);
    std::iota(all_slots.begin(), all_slots.end(), 0);

    //! model definition
    CpModelBuilder cp_model;

    //! variables definition
    // shifts[(n, d, s)] equals 1 if shift s is assigned to worker n on day d, and 0 otherwise
    std::map<std::tuple<int, int, int>, IntVar> shifts;
    for (int n : all_workers)
    {
        for (int d : all_slots)
        {
            for (int s : all_shifts)
            {
                auto key    = std::make_tuple(n, d, s);
                shifts[key] = cp_model.NewBoolVar().WithName(absl::StrFormat("shift_n%dd%ds%d", n, d, s));
            }
        }
    }

    //! each shift is assigned to a min-to-max workers per day, set 0 workers on Friday afternoon
    // [covid, residences, booking][monday - friday][min, opt, max]
    const auto& shiftsLimits = m_shiftsTableModel->Shifts();

    const QStringList& workers                  = m_skillHourModel->workersNames();
    const QMap<QString, WorkerSet>& workersSets = m_skillHourModel->workers();

    std::vector<IntVar> y;

    for (int d : all_slots) // ALL DAYS {MORNING, AFTERNOON}
    {
        // TODO - need to set year
        // TODO - move date validation to availability model
        const QDate& date = QDate::fromString(QString("%1 %2").arg(dates.at(d / 2)).arg(2021), "d.M. yyyy");

        Q_ASSERT(date.isValid());

        if (date.day() == 17) // a Czech national holiday
        {
            // TODO - a big hack for now
            continue;
        }

        const qint32 dayOfWeek = date.dayOfWeek();

        if (dayOfWeek == G::Saturday || dayOfWeek == G::Sunday)
        {
            continue; // no work during weekends
        }

        for (int s : all_shifts) // Residences, Booking, Covid
        {
            const auto& shiftLimits = shiftsLimits.at(s);

            std::vector<IntVar> x;
            for (int n : all_workers)
            {
                const QString& worker          = workers.at(n);
                const WorkerSet& workerSet     = workersSets.value(worker);
                const Skills::SkillLevel level = workerSet.m_skills.skills.at(s);
                // if worker has the skill, add him
                if (level > Skills::None)
                {
                    auto key = std::make_tuple(n, d, s);
                    x.push_back(shifts[key]);
                    y.push_back(shifts[key]);
                }
            }

            // make Fridays only mornings
            const bool afternoon = d % 2;
            if (dayOfWeek == G::Friday && afternoon)
            {
                cp_model.AddLessOrEqual(0, LinearExpr::Sum(x)); // zero workers on Friday afternoon
                cp_model.AddLessOrEqual(LinearExpr::Sum(x), 0);
            }
            else
            {

                cp_model.AddLessOrEqual(/*shiftLimits.at(dayOfWeek - 1).minimal*/ 0, LinearExpr::Sum(x)); // TODO - optimize min workers
                cp_model.AddLessOrEqual(LinearExpr::Sum(x), shiftLimits.at(dayOfWeek - 1).maximal);
            }
        }
    }

    cp_model.Maximize(LinearExpr::Sum(y));

    //    //! each slot need to have at least one senior worker, one project worker and one non-project worker
    //    for (int d : all_slots)
    //    {
    //        for (int s : all_shifts)
    //        {
    //            std::vector<IntVar> seniors;
    //            std::vector<IntVar> projects;
    //            std::vector<IntVar> nonProjects;
    //            for (int n : all_workers)
    //            {
    //                const QString& worker     = workers.at(n);
    //                const QStringList& skills = workersSets.value(worker);

    //                auto shiftKey = std::make_tuple(n, d, s);
    //                if (G::isSenior(skills))
    //                {
    //                    seniors.push_back(shifts[shiftKey]);
    //                }

    //                if (G::isProject(skills))
    //                {
    //                    projects.push_back(shifts[shiftKey]);
    //                }
    //                else
    //                {
    //                    nonProjects.push_back(shifts[shiftKey]);
    //                }
    //            }
    //            cp_model.AddGreaterOrEqual(LinearExpr::Sum(seniors), 1);     // is there at least one senior worker
    //            cp_model.AddGreaterOrEqual(LinearExpr::Sum(projects), 1);    // is there at least one project worker
    //            cp_model.AddGreaterOrEqual(LinearExpr::Sum(nonProjects), 1); // is there at least one non project worker
    //        }
    //    }

    //! workers hours fond
    for (int n : all_workers)
    {
        std::vector<IntVar> x;
        for (int s : all_shifts)
        {
            for (int d : all_slots)
            {
                auto key = std::make_tuple(n, d, s);
                x.push_back(shifts[key]);
            }
        }
        const QString& worker = workers.at(n);
        cp_model.AddLessOrEqual(LinearExpr::Sum(x), workersSets.value(worker).m_hours);
    }

    //! each worker works at most one slot according to his/her requirements
    const QMap<QString, QStringList>& workersAvailabitilty = m_availabilityModel->workersAvailabitilty();
    for (int n : all_workers)
    {
        const QString& worker              = workers.at(n);
        const QStringList& availabitilties = workersAvailabitilty.value(worker);

        if (availabitilties.size() != num_days)
        {
            qInfo() << "worker" << worker << "has invalid availabilities";
            continue;
        }

        for (int d : all_slots)
        {
            std::vector<IntVar> x;
            for (int s : all_shifts)
            {
                auto key = std::make_tuple(n, d, s);
                x.push_back(shifts[key]);
            }

            const QString& availabitilty = availabitilties.at(d / G::SlotsPerDay);
            const bool morning           = (d + 1) % G::SlotsPerDay;
            // respect workers requirements: if worker is not available, do not add it, otherwise he can work only one slot
            cp_model.AddLessOrEqual(LinearExpr::Sum(x), available(availabitilty, morning));

            if (morning && availabitilty == G::MorningOrAfternoonShift)
            {
                std::vector<IntVar> y;
                for (int s : all_shifts)
                {
                    auto key = std::make_tuple(n, d, s);
                    y.push_back(shifts[key]);
                }
                for (int s : all_shifts)
                {
                    auto key = std::make_tuple(n, d + 1, s);
                    y.push_back(shifts[key]);
                }
                cp_model.AddLessOrEqual(LinearExpr::Sum(y), 1);
            }
        }
    }

    //! assign shifts evenly
    //    const int min_shifts_per_nurse = (num_shifts * num_days) / num_workers;
    //    int max_shifts_per_nurse;
    //    if ((num_shifts * num_days) % num_workers == 0)
    //    {
    //        max_shifts_per_nurse = min_shifts_per_nurse;
    //    }
    //    else
    //    {
    //        max_shifts_per_nurse = min_shifts_per_nurse + 1;
    //    }
    //    for (int n : all_workers)
    //    {
    //        std::vector<IntVar> num_shifts_worked;
    //        // int num_shifts_worked = 0;
    //        for (int d : all_days)
    //        {
    //            for (int s : all_shifts)
    //            {
    //                auto key = std::make_tuple(n, d, s);
    //                num_shifts_worked.push_back(shifts[key]);
    //            }
    //        }
    //        cp_model.AddLessOrEqual(min_shifts_per_nurse, LinearExpr::Sum(num_shifts_worked));
    //        cp_model.AddLessOrEqual(LinearExpr::Sum(num_shifts_worked), max_shifts_per_nurse);
    //    }

    const CpSolverResponse response = Solve(cp_model.Build());

    if (response.status() == CpSolverStatus::OPTIMAL || response.status() == CpSolverStatus::FEASIBLE)
    {
        const QMap<QString, WorkerSet>& workersSets = m_skillHourModel->workers();
        const QStringList& workers                  = m_skillHourModel->workersNames();

        for (qint32 i = 0; i < num_shifts; ++i)
        {
            m_schedule[i].clear();
            m_schedule[i].resize(num_slots);
        }

        for (int d : all_slots)
        {
            for (int n : all_workers)
            {
                for (int s : all_shifts)
                {
                    auto key = std::make_tuple(n, d, s);
                    if (SolutionIntegerValue(response, shifts[key]))
                    {
                        m_schedule[s][d].push_back(n);

                        Q_ASSERT(workersSets.value(workers.at(n)).m_skills.skills.at(s) > Skills::None);
                    }
                }
            }
        }
    }

    // Statistics.
    LOG(INFO) << "Statistics";
    LOG(INFO) << CpSolverResponseStats(response);
}

bool Planner::Validate()
{
    //    const QStringList& workersNames             = m_skillHourModel->workersNames();
    //    const QStringList& availabilityWorkersNames = m_availabilityModel->workersNames();

    //    if (QSet<QString>(workersNames.cbegin(), workersNames.cend()) !=
    //        QSet<QString>(availabilityWorkersNames.cbegin(), availabilityWorkersNames.cend()))
    //    {
    //        qInfo() << "invalid data, inconsistent workers names";
    //        return false;
    //    }

    return true;
}

void Planner::Plan(AvailabilityTableModel* availabilityModel, WorkersModel* skillHourModel, ShiftsTableModel* shiftsTableModel)
{
    m_availabilityModel = availabilityModel;
    m_skillHourModel    = skillHourModel;
    m_shiftsTableModel  = shiftsTableModel;

    if (Validate())
    {
        // generate
        qDebug() << "planning...";
        ScheduleRequestsSat();

        qDebug() << "got it";

        emit Planned(m_schedule);
    }
}
