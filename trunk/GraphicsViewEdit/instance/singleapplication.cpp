/************************************************************************************
The SingleApplication component provides support for applications that can be 
only started once per user.

The SingleApplication class provides an interface to detect a running instance, 
and to send command strings to that instance.

Licence: GPL2
************************************************************************************/

#include "singleapplication.h"
#include "singleapplication_p.h"

#include <QtCore/QByteArray>
#include <QtCore/QDataStream>

#include <QtNetwork/QLocalSocket>

// timeouts are in ms
static const int i_timeout_connect	= 500;
static const int i_timeout_read		= 5000;
static const int i_timeout_write	= 5000;

static bool writeMessage(QLocalSocket* socket, const QString& message, int timeout = 5000)
{
	if(!socket || socket->state() != QLocalSocket::ConnectedState)
		return false;

	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out << (quint32)0;
	out << message;
	out.device()->seek(0);
	out << (quint32)(block.size() - sizeof(quint32));

	socket->write(block);
	if(!socket->waitForBytesWritten(timeout))
		return false;

	return true;
}

static QString readMessage(QLocalSocket* socket, bool* ok = 0)
{
	if(ok)
		*ok = false;

	if(!socket || socket->state() != QLocalSocket::ConnectedState)
		return QString();

	while(socket->bytesAvailable() < (int)sizeof(quint32))
	{
		if(!socket->waitForReadyRead(i_timeout_read))
			return QString();
	}

	QString message;
	{
		quint32 blockSize;
		QDataStream in(socket);
		in >> blockSize;
		while(socket->bytesAvailable() < blockSize)
		{
			if(!socket->waitForReadyRead(i_timeout_read))
				return message;
		}
		in >> message;
	}
	if(ok)
		*ok = true;

	return message;
}


LocalThread::LocalThread(quintptr socketDescriptor, const QString& name, QObject* parent) : QThread(parent)
{
	this->socketDescriptor = socketDescriptor;
	this->name = name;
}

LocalThread::~LocalThread()
{
}

void LocalThread::run()
{
	QLocalSocket socket;
	if(!socket.setSocketDescriptor(socketDescriptor) || socket.state() != QLocalSocket::ConnectedState)
		return;

	// send app_id to client
	if(!writeMessage(&socket, name))
		return;

	// wait to messages from client
	const QString magic = name + ":";
	while(socket.state() == QLocalSocket::ConnectedState && socket.waitForReadyRead())
	{
		while(socket.bytesAvailable() > 0)
		{
			QString message = readMessage(&socket);
			if(message.startsWith(magic))
				emit messageReceived(message.remove(0, magic.size()));
		}
	}
}


ThreadedLocalServer::ThreadedLocalServer(const QString& name, QObject* parent) : QLocalServer(parent)
{
	this->name = name;
}

ThreadedLocalServer::~ThreadedLocalServer()
{
}

void ThreadedLocalServer::incomingConnection(quintptr socketDescriptor)
{
	LocalThread* thread = new LocalThread(socketDescriptor, name, this);
	connect(thread, SIGNAL(messageReceived(const QString&)),
			 this, SIGNAL(messageReceived(const QString&)));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	thread->start();
}


SingleApplicationPrivate::SingleApplicationPrivate() :
	running(false),
	server(0), socket(0)
{
}

SingleApplicationPrivate::~SingleApplicationPrivate()
{
}

void SingleApplicationPrivate::init()
{
	running = false;
	socket = new QLocalSocket(q);
	socket->connectToServer(serverName);
	if(socket->waitForConnected(i_timeout_connect))
	{
		bool ok;
		const QString message = readMessage(socket, &ok);
		// now compare received bytes with name
		if(ok && message.compare(name) == 0)
			running = true;
	}

	if(!running)
	{
		socket->abort();
		delete socket;
		socket = 0;

		server = new ThreadedLocalServer(name, q);
		QObject::connect(server, SIGNAL(messageReceived(const QString&)),
				 q, SIGNAL(messageReceived(const QString&)), Qt::QueuedConnection);
		if(!server->listen(serverName))
		{
			delete server;
			server = 0;
		}
	}
}

bool SingleApplicationPrivate::sendMessage(const QString& message, int timeout)
{
	return running && writeMessage(socket, name + ":" + message, timeout);
}


SingleApplication::SingleApplication(const QString& name, QObject* parent) :
	QObject(parent), d(new SingleApplicationPrivate)
{
	d->q = this;
	d->name = name;
	d->serverName = "SingleApplication";
	d->init();
}

SingleApplication::SingleApplication(const QString& name, const QString& serverName, QObject* parent) :
	QObject(parent), d(new SingleApplicationPrivate)
{
	d->q = this;
	d->name = name;
	d->serverName = serverName;
	d->init();
}

SingleApplication::~SingleApplication()
{
}

QString SingleApplication::name() const
{
	return d->name;
}

QString SingleApplication::serverName() const
{
	return d->serverName;
}

bool SingleApplication::isRunning() const
{
	return d->running;
}

bool SingleApplication::sendMessage(const QString& message, int timeout)
{
	return d->sendMessage(message, timeout);
}

bool SingleApplication::isRunning(const QString& name)
{
	return SingleApplication(name).isRunning();
}

bool SingleApplication::sendMessage(const QString& name, const QString& message, int timeout)
{
	return SingleApplication(name).sendMessage(message, timeout);
}
