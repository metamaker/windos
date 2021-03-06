#include "TaskFactory.h"

#include <utility/StringConversions.h>

using todos_model_factory::TaskFactory;
using todos_model_entity::Task;
using todos_model_traits::TaskTraits;
using todos_model_traits::AreFieldsFromSameEntity;

TaskFactory::EntitySharedPtr TaskFactory::CreateFromFieldsValues(const TaskFactory::FieldsValuesContainer &values) const
{
  EntitySharedPtr ptrEntity;

  if (AreFieldsFromSameEntity(TaskTraits().GetFieldsNames(), values)) {
    unsigned int id = todos_utility::StringToInt(values.at(TaskTraits().GetIdFieldName()));
    unsigned int categoryId = todos_utility::StringToInt(values.at("category_id"));
    std::string title = values.at("title");
    Task::Priority priority = StringToPriority(values.at("priority"));
    Task::DateTime creationDate = todos_utility::StringToDateTime(values.at("creation_date"));
    Task::DateTime dueDate = todos_utility::StringToDateTime(values.at("due_date"));
    Task::DateTime reminderDate = todos_utility::StringToDateTime(values.at("reminder_date"));
    Task::Status status = StringToStatus(values.at("status"));

    ptrEntity.reset(new Task(id, categoryId, title, priority, creationDate, dueDate, reminderDate, status));
  }

  return ptrEntity;
}

TaskFactory::FieldsValuesContainer TaskFactory::RevertToFieldsValues(const TaskFactory::Entity &entity) const
{
  FieldsValuesContainer values;

  values.insert(std::make_pair(TaskTraits().GetIdFieldName(), todos_utility::IntToString(entity.GetId())));
  values.insert(std::make_pair("category_id", todos_utility::IntToString(entity.GetCategoryId())));
  values.insert(std::make_pair("title", entity.GetTitle()));
  values.insert(std::make_pair("priority", PriorityToString(entity.GetPriority())));
  values.insert(std::make_pair("creation_date", todos_utility::DateTimeToString(entity.GetCreationDate())));
  values.insert(std::make_pair("due_date", todos_utility::DateTimeToString(entity.GetDueDate())));
  values.insert(std::make_pair("reminder_date", todos_utility::DateTimeToString(entity.GetReminderDate())));
  values.insert(std::make_pair("status", StatusToString(entity.GetStatus())));

  return values;
}

TaskFactory::TaskFactory()
{
  m_priorityMapping.push_back(std::make_pair("1-low", Task::Priority::LOW));
  m_priorityMapping.push_back(std::make_pair("2-normal", Task::Priority::NORMAL));
  m_priorityMapping.push_back(std::make_pair("3-high", Task::Priority::HIGH));

  m_statusMapping.push_back(std::make_pair("uncompleted", Task::Status::UNCOMPLETED));
  m_statusMapping.push_back(std::make_pair("completed", Task::Status::COMPLETED));
}

std::string TaskFactory::PriorityToString(Task::Priority priority) const
{
  for (auto i = m_priorityMapping.begin(), iend = m_priorityMapping.end(); i != iend; ++i) {
    if (i->second == priority) {
      return i->first;
    }
  }

  return "";
}

Task::Priority TaskFactory::StringToPriority(const std::string &priority) const
{
  for (auto i = m_priorityMapping.begin(), iend = m_priorityMapping.end(); i != iend; ++i) {
    if (i->first == priority) {
      return i->second;
    }
  }

  return Task::Priority::UNKNOWN;
}

std::string TaskFactory::StatusToString(Task::Status status) const
{
  for (auto i = m_statusMapping.begin(), iend = m_statusMapping.end(); i != iend; ++i) {
    if (i->second == status) {
      return i->first;
    }
  }

  return "";
}

Task::Status TaskFactory::StringToStatus(const std::string &status) const
{
  for (auto i = m_statusMapping.begin(), iend = m_statusMapping.end(); i != iend; ++i) {
    if (i->first == status) {
      return i->second;
    }
  }

  return Task::Status::UNKNOWN;
}
