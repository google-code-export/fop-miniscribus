/************************************************************************************
The SingleApplication component provides support for applications that can be 
only started once per user.

The SingleApplication class provides an interface to detect a running instance, 
and to send command strings to that instance.

Licence: GPL2
************************************************************************************/

#ifndef SINGLE_APPLICATION_H
#define SINGLE_APPLICATION_H

#include <QtCore/QObject>
#include <QtCore/QString>

class SingleApplicationPrivate;
class SingleApplication : public QObject
{
	Q_OBJECT

public:
	explicit SingleApplication(const QString& name, QObject* parent = 0);
	SingleApplication(const QString& name, const QString& serverName, QObject* parent = 0);
	~SingleApplication();

signals:
	void messageReceived(const QString& message);

public:
	QString name() const;
	QString serverName() const;

	bool isRunning() const;

	static bool isRunning(const QString& name);
	static bool sendMessage(const QString& name, const QString& message, int timeout = 5000);

public slots:
	bool sendMessage(const QString& message, int timeout = 5000);

private:
	friend class SingleApplicationPrivate;

	SingleApplicationPrivate* d;
};

#endif // SINGLE_APPLICATION_H
