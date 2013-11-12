#ifndef TASK_H
#define TASK_H

#include <QTime>

struct Task {
  QDate date;
  QTime time;
  QString description;
};

inline bool operator==(const Task& lhs, const Task& rhs){
  return (lhs.date == rhs.date) && (lhs.time == rhs.time) && (lhs.description == rhs.description);
}

inline bool operator<(const Task& lhs, const Task& rhs){
  if (lhs.date == rhs.date) {
    return lhs.time < rhs.time;
  } else {
    return lhs.date < rhs.date;
  }
}

#endif // TASK_H
