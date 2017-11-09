#ifndef DBWRITER_HPP
#define DBWRITER_HPP
#include <thread>
#include "state.hpp"

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/gridfs/bucket.hpp>

template< typename T >
class DBWriter{
  private:
    mongocxx::instance m_inst;
    std::string m_host;
    std::string m_db;
    T m_params;
    State* m_s;
  public:

    DBWriter(State *_s, std::string _hostalias, std::string _db, T _params )
      : m_s(_s), m_db(_db), m_params(_params)
      {
        m_host = "mongodb://"+_hostalias+":27017";
      }
    bsoncxx::builder::stream::document get_metaBSON() {
    	bsoncxx::builder::stream::document metaBSON{};
    	for( const auto &p : m_params) {
    		metaBSON << p.first << p.second;
    	}
    	return metaBSON;
    }

    void write_all() {
      std::thread th1 ( &DBWriter<T>::write_history, this );
      std::thread th2 ( &DBWriter<T>::write_energy, this  );
      th1.join();
      th2.join();


    }

    void write_energy() {

      mongocxx::uri uri(m_host);
    	mongocxx::client conn(uri);
    	auto db = conn[m_db];
      auto collection = db["data"];
      auto builder = get_metaBSON();
      auto enr_array = builder << "Energy" << bsoncxx::builder::stream::open_array;
    	for(const auto &energy : m_s->energy_history()) {

    	  enr_array << energy;

    	}
    	auto after_enr_array = enr_array << bsoncxx::builder::stream::close_array;
    	auto energy_doc = after_enr_array << bsoncxx::builder::stream::finalize;
      auto energy_result = collection.insert_one(energy_doc.view());
    }
    void write_vector(std::string key, std::vector<double> v) {

      mongocxx::uri uri(m_host);
    	mongocxx::client conn(uri);
    	auto db = conn[m_db];
      auto collection = db["data"];
      auto builder = get_metaBSON();
      auto array = builder << key << bsoncxx::builder::stream::open_array;
    	for(const auto &value : v) {

    	  array << value;

    	}
    	auto after_array = array << bsoncxx::builder::stream::close_array;
    	auto doc = after_array << bsoncxx::builder::stream::finalize;
      auto result = collection.insert_one(doc.view());

    }
    void write_history() {

    	mongocxx::uri uri(m_host);
    	mongocxx::client conn(uri);
    	auto db = conn[m_db];
    	auto collection = db["data"];

    	auto meta_doc = get_metaBSON() << bsoncxx::builder::stream::finalize;
    	mongocxx::v_noabi::options::gridfs::upload options;
    	options.metadata(meta_doc.view());

    	auto bucket = db.gridfs_bucket();
    	auto uploader = bucket.open_upload_stream( "configuration_file" , options );
    	const int N = m_s->size() * m_s->steps() * sizeof(double);
    	std::uint8_t history[ N ];

    	union{
    		double m_double;
    		std::uint8_t m_byte[sizeof(double)];
    	} convert;

    	int i = 0;
    	for(const auto &configuration : m_s->history()){
    		for(const auto &site : configuration){
    			convert.m_double = site;
    			for(const auto &byte : convert.m_byte){
    				history[i++] = byte;
    			}
    		}
    	}
    	uploader.write(history, N);
    	uploader.close();
    }

};
#endif