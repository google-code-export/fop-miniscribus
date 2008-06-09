/************************************************************************************
The SingleApplication component provides support for applications that can be 
only started once per user.

The SingleApplication class provides an interface to detect a running instance, 
and to send command strings to that instance.

Licence: GPL2
************************************************************************************/

#ifndef SINGLE_APPLICATION_P_H
#define SINGLE_APPLICATION_P_H

#include "singleapplication.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QThread>

#include <QtNetwork/QLocalServer>

class QLocalSocket;

class SingleApplicationPrivate
{
	friend class SingleApplication;

public:
	SingleApplicationPrivate();
	~SingleApplicationPrivate();

	void init();
	bool sendMessage(const QString& message, int timeout);

	QString name;
	QString serverName;
	bool running;

	SingleApplication* q;

	QLocalServer* server;
	QLocalSocket* socket;
};


class LocalThread : public QThread
{
	Q_OBJECT

public:
	LocalThread(quintptr socketDescriptor, const QString& name, QObject* parent = 0);
	virtual ~LocalThread();

signals:
	void messageReceived(const QString& message);

protected:
	void run();

private:
	quintptr socketDescriptor;
	QString name;
};


class ThreadedLocalServer : public QLocalServer
{
	Q_OBJECT

public:
	ThreadedLocalServer(const QString& name, QObject* parent = 0);
	virtual ~ThreadedLocalServer();

signals:
	void messageReceived(const QString& message);

protected:
	void incomingConnection(quintptr socketDescriptor);

private:
	QString name;
};

#endif // SINGLE_APPLICATION_P_H
