#include "interpreterBase/robotModel/commonRobotModel.h"

#include <qrkernel/exception/exception.h>
#include <utils/realTimer.h>

#include "interpreterBase/robotModel/robotParts/touchSensor.h"

using namespace interpreterBase::robotModel;

CommonRobotModel::CommonRobotModel()
	: mState(disconnectedState)
	, mConfigurationPostponed(false)
{
	connect(&mConfiguration, &Configuration::allDevicesConfigured
			, this, &CommonRobotModel::allDevicesConfigured, Qt::QueuedConnection);

	connect(this, &CommonRobotModel::connected, this, &CommonRobotModel::onConnected);
	connect(this, &CommonRobotModel::disconnected, this, &CommonRobotModel::onDisconnected);
}

CommonRobotModel::~CommonRobotModel()
{
	stopRobot();
}

void CommonRobotModel::init()
{
	rereadSettings();
}

void CommonRobotModel::connectToRobot()
{
	emit connected(true);
}

void CommonRobotModel::stopRobot()
{
}

void CommonRobotModel::disconnectFromRobot()
{
	emit disconnected();
}

CommonRobotModel::ConnectionState CommonRobotModel::connectionState() const
{
	return mState;
}

ConfigurationInterface &CommonRobotModel::mutableConfiguration()
{
	return mConfiguration;
}

void CommonRobotModel::onConnected(bool success)
{
	if (success) {
		mState = connectedState;

		if (mConfigurationPostponed) {
			mConfiguration.applyConfiguration();
			mConfigurationPostponed = false;
		}
	} else {
		mState = disconnectedState;
	}
}

void CommonRobotModel::onDisconnected()
{
	mState = disconnectedState;
}

ConfigurationInterface const &CommonRobotModel::configuration() const
{
	return mConfiguration;
}

QList<PortInfo> CommonRobotModel::availablePorts() const
{
	return mAllowedConnections.keys();
}

QList<PortInfo> CommonRobotModel::configurablePorts() const
{
	return availablePorts();
}

QList<DeviceInfo> CommonRobotModel::allowedDevices(PortInfo const &port) const
{
	return mAllowedConnections[port];
}

void CommonRobotModel::configureDevice(PortInfo const &port, DeviceInfo const &deviceInfo)
{
	robotParts::Device *device = createDevice(port, deviceInfo);
	if (device) {
		mConfiguration.configureDevice(device);
	}
	/// @todo Handle error
}

void CommonRobotModel::applyConfiguration()
{
	if (mState == connectedState) {
		mConfiguration.applyConfiguration();
	} else {
		mConfigurationPostponed = true;
	}
}

QList<DeviceInfo> CommonRobotModel::convertibleBases() const
{
	return {};
}

utils::AbstractTimer *CommonRobotModel::produceTimer()
{
	return new utils::RealTimer;
}

void CommonRobotModel::rereadSettings()
{
}

void CommonRobotModel::addAllowedConnection(PortInfo const &port, QList<DeviceInfo> const &devices)
{
	mAllowedConnections[port].append(devices);
}

robotParts::Device * CommonRobotModel::createDevice(
		PortInfo const &port, DeviceInfo const &deviceInfo)
{
	Q_UNUSED(port);
	Q_UNUSED(deviceInfo);

	/// @todo Handle error?
	return nullptr;
}
