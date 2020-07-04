# messagesparser
Is a tool used to filter, transform and output information from various input files.
Initially used over unix based messages log files coming from syslog, rsyslog etc.

It is being designed and implemented in order to support dynamic file formats without
the need to change anything to the code it self, by providing its own configuration
system. It supports different types of outputs such as organized and structured text & xml files.

Its dynamic ability to analyze dynamic formats gives the ability to parse any log files, regardless
where they come from, such as log files from:
- Apache
- Nginx
- Mysql
- Unix Based Messages log files etc.


The purpose of this library is to support (in the future) the following areas:

- Statistical analysis (graphical representation of the information)
- Monitoring capabilities
- Pattern maching
- Support for streams such as sockets, pipes
- Support for multiple and different sources in the same time
- Multiple platform support
- Various Databases support (MySQL, PostgreSQL, MsSQL Server, Oracle)
