#include "planner.h"

#include <absl/strings/str_format.h>
#include <ortools/sat/cp_model.h>
#include <ortools/sat/model.h>
#include <ortools/sat/sat_parameters.pb.h>
#include <ortools/util/time_limit.h>

#include <QDebug>

Planner::Planner(QObject* parent) : QObject(parent)
{
    operations_research::sat::CpModelBuilder cp_model;
}

void Planner::Plan(AvailabilityTableModel* availabilityModel, SkillHourTableModel* skillHourModel)
{
    qDebug() << "hello world";

    // validate
    // generate
}
