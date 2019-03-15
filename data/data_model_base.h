#pragma once

#include "requirements.h"
#include <iostream>

template<typename T>
class data_model_base
{
	using json=nlohmann::json;

	using string=std::string;
	using pair=std::pair<T,T>;
	using vector=std::vector<pair>;

	using size_t=unsigned int;

	virtual string  _version() { return string("version"); }
	virtual string _name() { return string("name"); }
	virtual string _comment() { return string("comment"); }
	virtual string _count() { return string("count"); }
	virtual string _data() { return string("data"); }
	virtual string _first_name() { return string(""); };
	virtual string _second_name() { return string(""); };

public:
	data_model_base()
	{
		count = 0;
	}
	virtual ~data_model_base(){}
	
	string version;
	string name;
	string comment;

	size_t count;
	vector data;

	virtual void load_from_file(const string& path) final
	{
		std::ifstream input_file;
		json j;
		try
		{
			input_file.open(path);
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			std::cerr << "�ļ���ʧ��" << std::endl;
			return;
		}
		try
		{
			input_file >> j;
			input_file.close();
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			std::cerr << "����JSONʧ��" << " " << path << std::endl;
			return;
		}
		load_from_json(j);
	}
	virtual void save_to_file(const string& path) final
	{
		std::ofstream output_file;
		auto j = save_to_json();
		try
		{
			output_file.open(path);
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			std::cerr << "�ļ���ʧ��" << std::endl;
			return;
		}
		try
		{
			output_file << j.dump(4);
			output_file.close();
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			std::cerr << "�ļ�д��ʧ��" << " " << path << std::endl;
		}
	}
	virtual void load_from_json(const json& j)
	{
		version = j[_version()].get<string>();
		name = j[_name()].get<string>();
		comment = j[_comment()].get<string>();
		count = j[_count()].get<T>();

		data.clear();
		json data_j = j[_data()];
		for(auto it=data_j.begin();it!=data_j.end();++it)
		{
			T first_data = (*it)[_first_name()].get<T>();
			T second_data = (*it)[_second_name()].get<T>();
			data.emplace_back(first_data, second_data);
		}
	}
	virtual json save_to_json()
	{
		json j;
		j[_version()] = version;
		j[_name()] = name;
		j[_comment()] = comment;
		j[_count()] = count;

		json data_j;
		for(auto it=data.begin();it!=data.end();++it)
		{
			json unit_j;
			unit_j[_first_name()] = it->first;
			unit_j[_second_name()] = it->second;
			data_j.emplace_back(unit_j);
		}
		j[_data()] = std::move(data_j);
		return j;
	}
};
