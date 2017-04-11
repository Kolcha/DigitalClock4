/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2017  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <execinfo.h>
#include <cxxabi.h>
#include <signal.h>

#include <stdlib.h>

#include <QRegularExpression>
#include <QDateTime>
#include <QDir>
#include <QFile>


static void stack_trace()
{
  const int max_addrs = 128;
  void* addrlist[max_addrs];
  int addrlen = backtrace(addrlist, max_addrs);
  if (addrlen == 0) return;

  QDateTime now = QDateTime::currentDateTime();
  QString filename = QString("digital_clock_crash_%1.log").arg(now.toString("dd-MM-yyyy_HH-mm-ss"));
  QFile log_file(QDir::home().absoluteFilePath(filename));
  if (!log_file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

  char** symbollist = backtrace_symbols(addrlist, addrlen);

#if defined(Q_OS_MACOS)
  QRegularExpression re("\\d+\\s+(?<mod>\\S+)\\s+(?<addr>\\S+)\\s+(?<func>\\S+)\\s*\\+\\s*(?<off>\\S+)");
#elif defined(Q_OS_LINUX)
  QRegularExpression re("(?<mod>.*)\\((?<func>\\S+)\\+(?<off>\\S+)\\)\\s*\\[(?<addr>.*)\\]");
#else
  QRegularExpression re;
#endif

  for (int i = 1; i < addrlen; i++) {
    QRegularExpressionMatch match = re.match(symbollist[i]);
    if (re.captureCount() > 0 && match.hasMatch()) {
      QString module = match.captured("mod") + " ";
      log_file.write(module.toUtf8());
      QString address = "[" + match.captured("addr") + "] ";
      log_file.write(address.toUtf8());
      QString funct = match.captured("func");
      int status;
      char* ret = abi::__cxa_demangle(funct.toStdString().c_str(), NULL, 0, &status);
      if (status == 0) {
        funct = QString(ret);
        free(ret);
      }
      log_file.write(funct.toUtf8());
      QString offset = " + " + match.captured("off") + "\n";
      log_file.write(offset.toUtf8());
    } else {
      QString line(symbollist[i]);
      line += '\n';
      log_file.write(line.toUtf8());
    }
  }

  log_file.close();
  free(symbollist);
}


static void sig_handler(int sig)
{
  stack_trace();
  exit(128 + sig);
}


void init_unix_backtrace()
{
  signal(SIGABRT, sig_handler);
  signal(SIGSEGV, sig_handler);
}
