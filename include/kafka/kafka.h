#pragma once
#include <common.h>
#include <glib.h>
#include <librdkafka/rdkafka.h>
#include <thread>
#include <string>
#include <kafka_events.h>

// Abstract class that all the Kafka Related Objects inherit from to ensure consistent setup 
// of objects in the code.
class KafkaObject {
protected:
	rd_kafka_conf_t* config_d; // hold the configuration for all the kafka objects.
	GKeyFile* keyFile;
	char errstr[512];
	bool exit;
	std::string service_id;
	std::string service_name;

	KafkaObject(const char* config_path, std::string s_id, std::string s_name);
	KafkaObject(GKeyFile*, std::string s_id, std::string s_name);
	KafkaObject() = default;
	~KafkaObject();

	virtual void config(const gchar* group); // this will handle object specific configurations
	virtual void init() = 0;

	std::string getServiceID() { return service_id; }
	std::string getServiceName() { return service_name; }
};

class Producer : public KafkaObject {

public:
	Producer(GKeyFile* keyFile, std::string s_id, std::string s_name);
	~Producer();

	void init() override;
	void publish(const gchar* topic, const gchar* key, const gchar* value);
	void publish(std::string topic, std::string kafka_key_type, std::string payload);
	EventsManager& getAssociatedEManager();
	void registerService(std::string topic); // this function must publish a message to the services topic

private:
	rd_kafka_t* producer;
	long failed_count = 0;
	EventsManager events_manager;
};

class Consumer : public KafkaObject {

public:
	Consumer(GKeyFile* keyFile, std::string s_id, std::string s_name);
	~Consumer();

	void init() override;
	void subscribe(const char* topic);
	void subscribe(std::string topic);
	void startPolling();
	void exitPolling();
	void registerEventManager(EventsManager* event_manager);
private:
	rd_kafka_t* consumer;
	std::thread* worker;
	EventsManager* events_manager;
	void poll();
};


class Kafka: public KafkaObject{
	// This class will form the interface to which you receive you Consumer and Recevier.
public:
	Kafka(const char* config_path, std::string s_id, std::string s_name);
	void init() override;
	Producer* createProducer();
	Consumer* createConsumer();
private:
};


