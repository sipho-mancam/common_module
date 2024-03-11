#pragma once
#include <iostream>
#include <kafka_events.h>

template <class T>
void printVector(std::vector<T> v)
{
	std::cout << "{ ";
	for (auto& a : v)
	{
		std::cout << a << ", ";
	}

	std::cout << " }" << std::endl;;
}

void testEventObject()
{
	std::string input_string = "{"
		"event_id:11232314567,"
		"service_id:asdhh123,"
		"event_type:INPUT_SHUTDOWN,"
		"service_name:Video Input System,"
		"time_stamp:234.0021,"
		"date:22-02-2024,"
		"data:{"
		"key1:value;"
		"key2:value2;"
		"key3:23;"
		"key4:254.123;"
		"key5:12.2 31.0 21.5 6 2;"
		"key6:45 23 12 5.8;"
		"key7:sthe man cam;"
		"}"
		"}";

	EventObject event_object(input_string);

	std::cout << event_object.getServiceID() << std::endl;
	std::cout << event_object.getServiceName() << std::endl;
	std::cout << event_object.getTimeStamp() << std::endl;
	std::cout << event_object.getEventDate() << std::endl;
	std::cout << event_object.getEventID() << std::endl;
	std::cout << event_object.getEventType() << std::endl;

	std::cout << event_object.getString("key1") << std::endl;
	std::cout << event_object.getInt("key3") << std::endl;
	std::cout << event_object.getFloat("key4") << std::endl;

	printVector<std::string>(event_object.getStringList("key7"));
	printVector<int>(event_object.getIntList("key6"));
	printVector<float>(event_object.getFloatList("key5"));
}