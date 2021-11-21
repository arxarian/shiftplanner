#include "planner.h"

#include "../global.h"

#include <absl/strings/str_format.h>
#include <ortools/sat/cp_model.h>
#include <ortools/sat/model.h>
#include <ortools/sat/sat_parameters.pb.h>
#include <ortools/util/time_limit.h>

#include <QDebug>

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
    //! data definition
    const int num_workers       = m_skillHourModel->rowCount();
    const int num_shifts        = 3;
    const int num_slots_per_day = 2;
    const int num_days          = 2;
    const int num_slots         = num_days * num_slots_per_day;

    Q_ASSERT(G::ShiftsNames.size() == num_shifts);

    std::vector<int> all_workers(num_workers);
    std::iota(all_workers.begin(), all_workers.end(), 0);

    std::vector<int> all_shifts(num_shifts);
    std::iota(all_shifts.begin(), all_shifts.end(), 0);

    std::vector<int> all_slots(num_slots);
    std::iota(all_slots.begin(), all_slots.end(), 0);

    //! model definition
    CpModelBuilder cp_model;

    //! variables definition
    // shifts[(n, d, s)] equals 1 if shift s is assigned to nurse n on day d, and 0 otherwise
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

    //! each shift is assigned to a min-to-max workers per day
    const std::array min_workers = {1, 1, 2}; // Covid, Booking, Residences
    const std::array max_workers = {3, 1, 5}; // Covid, Booking, Residences

    const QStringList& workers                     = m_skillHourModel->workers();
    const QMap<QString, QStringList>& workerSkills = m_skillHourModel->workersSkills();

    for (int d : all_slots)
    {
        for (int s : all_shifts)
        {
            std::vector<IntVar> x;
            for (int n : all_workers)
            {
                // if worker does not have the skill, do not add it
                if (!workerSkills.value(workers.at(n)).at(s + 2).isEmpty()) // the first two skills are "Senior" and "Project"
                {
                    auto key = std::make_tuple(n, d, s);
                    x.push_back(shifts[key]);
                }
            }
            cp_model.AddLessOrEqual(min_workers[s], LinearExpr::Sum(x));
            cp_model.AddLessOrEqual(LinearExpr::Sum(x), max_workers[s]);
        }
    }

    //! each worker works at most one shift per day
    for (int n : all_workers)
    {
        for (int d : all_slots)
        {
            std::vector<IntVar> x;
            for (int s : all_shifts)
            {
                auto key = std::make_tuple(n, d, s);
                x.push_back(shifts[key]);
            }
            cp_model.AddLessOrEqual(LinearExpr::Sum(x), 1);
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

    Model model;
    SatParameters parameters;
    parameters.set_linearization_level(0);
    // Enumerate all solutions.
    parameters.set_enumerate_all_solutions(false);
    model.Add(NewSatParameters(parameters));

    std::atomic<bool> stopped(false);
    //    model.GetOrCreate<operations_research::TimeLimit>()->RegisterExternalBooleanAsLimit(&stopped);

    model.Add(NewFeasibleSolutionObserver([&](const CpSolverResponse& r) {
        const QStringList& workers = m_skillHourModel->workers();
        for (int d : all_slots)
        {
            LOG(INFO) << "Day " << std::to_string(d / 2) << " - " << G::PartsNames.at(d % 2).toStdString();
            for (int n : all_workers)
            {
                bool is_working = false;
                for (int s : all_shifts)
                {
                    auto key = std::make_tuple(n, d, s);
                    if (SolutionIntegerValue(r, shifts[key]))
                    {
                        is_working = true;
                        LOG(INFO) << "  " << workers.at(n).toStdString() << " works " << G::ShiftsNames.at(s).toStdString();
                    }
                }
                //                if (!is_working)
                //                {
                //                    LOG(INFO) << "  " << workers.at(n).toStdString() << " does not work";
                //                }
            }
        }
        stopped = true;
    }));

    const CpSolverResponse response = SolveCpModel(cp_model.Build(), &model);

    // Statistics.
    LOG(INFO) << "Statistics";
    LOG(INFO) << CpSolverResponseStats(response);
}

void Planner::Plan(AvailabilityTableModel* availabilityModel, SkillHourTableModel* skillHourModel)
{
    m_availabilityModel = availabilityModel;
    m_skillHourModel    = skillHourModel;

    // validate

    // generate
    qDebug() << "planning...";
    ScheduleRequestsSat();

    qDebug() << "got it";
}
