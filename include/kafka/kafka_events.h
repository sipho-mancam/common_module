#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <kafka_event_constants.h>
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
struct isVector : std::false_type {};

template <typename T, typename Alloc>
struct isVector<std::vector<T, Alloc>> : std::true_type {};

template <typename T>
bool checkIfVector(const T& obj) {
	return isVector<T>::value;
}

template <class T>
static std::vector<T> convertToList(T elem)
{
	return std::vector<T>({ elem });
}

template <class T2>
static std::vector<std::string> convertToStringVector(std::vector<T2> v)
{
	std::vector<std::string> output_vector;

	for (auto& elem : v)
	{
		output_vector.push_back(std::to_string(elem));
	}

	return output_vector;
}

static std::string constructEventDataString(std::vector<std::string> keys, std::vector<std::vector<std::string>> values)
{
	std::string output_String = "{";
	if (keys.size() != values.size())
	{
		std::cerr << "Keys size != Values Size" << std::endl;
		return "";
	}

	// generate key:value; types for data
	for (int i = 0; i < keys.size(); i++)
	{
		std::string key = keys[i];
		if (values[i].size() > 1)
		{
			output_String += key + ":";
			for (auto& elem : values[i])
			{
				output_String += elem + " ";
			}
			output_String.pop_back();
			output_String +=";"; // close the list	
		}
		else {
			output_String += key+std::string(":")+values[i][0]+std::string(";");
		}
	}
	output_String += "}";
	return output_String;
}


struct ThreadNotifier {
public:
	void wait() {
		std::unique_lock<std::mutex> lock(mutex_);
		conditionVar_.wait(lock);
		
	}

	void waitFor(int ms) {
		std::unique_lock<std::mutex> lock(mutex_);
		conditionVar_.wait_for(lock, std::chrono::milliseconds(ms));
	}

	void notify() {
		std::lock_guard<std::mutex> lock(mutex_);
		conditionVar_.notify_one();
	}

	void notifyAll() {
		std::lock_guard<std::mutex> lock(mutex_);
		conditionVar_.notify_all();
	}

private:
	std::mutex mutex_;
	std::condition_variable conditionVar_;
};


class EventObject {
public:
	EventObject(std::string raw_e_data);
	// this is used to only search inside the data not the whole event structure values.
	std::string getString(std::string key);
	int getInt(std::string key);
	float getFloat(std::string key);

	std::vector<int> getIntList(std::string key);
	std::vector<float> getFloatList(std::string key);
	std::vector<std::string> getStringList(std::string key);

	std::vector<std::string> getKeys();
	std::vector<std::string> getDataKeys();
	std::string getServiceID();
	std::string getServiceName();
	std::string getEventDate();
	std::string getEventType();
	long long getEventID();
	std::string getDataRaw();
	double getTimeStamp();

	void printEvent();

private:
	std::string raw_event_data;
	std::vector<std::string> keys;
	std::vector<std::string>data_keys;
	std::string raw_data;
	std::vector<std::unordered_map<std::string, std::string>> data_key_values;
	std::unordered_map<std::string, std::string> event_key_values;
	void parseRawData(); // this will place param in variables for ease of access.
	std::unordered_map<std::string, std::string> parseToStringMap(std::string);
	std::vector<std::string> splitString(std::string& s, const char);
	std::string strip(std::string& i);

};




// the instance of this 
class EventsManager {
public:
	std::string service_id;
	std::string service_name;
	std::queue<EventObject> events;
	std::unordered_map<std::string, std::vector<std::function<void(EventObject&)>>> listeners_map;
	std::unordered_map<std::string, ThreadNotifier*> waiting_list; // all objects that are waiting for some event

	EventsManager(std::string s_id, std::string s_name);
	std::string createEvent(std::string event_type, std::string data);

	~EventsManager();

	void addEvent(std::string event_string); // this will be mostly used by the consumer object to pipe in events.
	void addEventListener(std::string event_type, const std::function<void(EventObject&)>&);
	void startEventLoop(); // this loop will handle events in the events queue and match them with their listeners or drop then entirely.
	void stopEventLoop();
	void waitForEvent(std::string event_type);  // synchronous way for an event.
	bool isRunning() { return stop_loop; }

private:

	bool stop_loop;
	std::thread* worker_thread;

	long long getCurrentTimeMillis() {
		auto currentTime = std::chrono::system_clock::now();
		auto duration = currentTime.time_since_epoch();
		return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	}

	std::string getCurrentDate() {
		// Get the current time
		std::time_t currentTime = std::time(nullptr);
		struct std::tm localTime;

		// Use localtime_s to avoid unsafe localtime
		if (localtime_s(&localTime, &currentTime) != 0) {
			// Handle error if localtime_s fails
			return "Error: Unable to get the current date";
		}
		// Format the date as a string
		std::ostringstream oss;
		oss << std::put_time(&localTime, "%d-%m-%Y");

		return oss.str();
	}

	void worker();
};
