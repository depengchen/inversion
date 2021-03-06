#pragma once

#include <type_traits>
#include <ios>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <utility>

#include <nlohmann/json.hpp>

#include "../global/global.h"

//地电模型与正演结果基类
class data_model_base
{
protected:
	using float_t = global::float_t;
	using string = std::string;
	using vector = global::vector;
	using size_type = vector::size_type;
	using json = nlohmann::json;

	static constexpr char index_name[] = "idx";
	static constexpr char default_version[] = "0.3.5";
	static constexpr char default_comment[] = "";

	static void throw_critical_data_miss_exception(const string& data_name)
	{
		throw std::runtime_error(
			global::msg("critical data ", data_name, " does not exist"));
	}

	static void ordinary_data_miss(const string& data_name)
	{
		global::err("data ", data_name, " does not exist");
	}

	virtual void load_version(const json& j)
	{
		if (!j.count(_version()))
		{
			ordinary_data_miss(_version());
			version = default_version;
			return;
		}
		version = j[_version()].get<string>();
	}

	virtual void load_name(const json& j)
	{
		if (!j.count(_name()))
		{
			throw_critical_data_miss_exception(_name());
		}
		name = j[_name()].get<string>();
	}

	virtual void load_comment(const json& j)
	{
		if (!j.count(_comment()))
		{
			ordinary_data_miss(_comment());
			comment = default_comment;
			return;
		}
		comment = j[_comment()].get<string>();
	}

	virtual void load_count(const json& j)
	{
		if (!j.count(_count()))
		{
			throw_critical_data_miss_exception(_count());
		}
		count = j[_count()].get<size_type>();
	}

	virtual void load_data(const json& j)
	{
		if (!j.count(_data()))
		{
			throw_critical_data_miss_exception(_data());
		}
		auto data_j = j[_data()];
		auto data_names = _data_content_name();
		data.clear();
		data.resize(_data_content_count());

		const auto c = j[_count()].get<size_type>();
		for (auto& item : data)
			item.resize(c);
		try
		{
			for (auto& item_j : data_j)
			{
				const auto idx = static_cast<size_type>(std::round(item_j[index_name].get<float_t>() - 1));
				for (size_type i = 0; i < _data_content_count(); ++i)
				{
					auto temp = item_j[data_names[i]].get<float_t>();
					data[i][idx] = temp;
				}
			}
		}
		catch (std::out_of_range& e)
		{
			global::err(e.what());
			global::err("count or idx is wrong");
			throw;
		}
	}

	virtual string _version() { return string("version"); }
	virtual string _name() { return string("name"); }
	virtual string _comment() { return string("comment"); }
	virtual string _count() { return string("count"); }
	virtual string _data() { return string("data"); }

	size_type _data_content_count() { return _data_content_name().size(); }
	virtual std::vector<string> _data_content_name() { return {index_name}; }

	virtual void load_additional_data(const json& j)
	{
	}

	virtual json save_additional_data() { return json(R"({})"); }

public:
	string version;
	string name;
	string comment;

	size_type count;
	std::vector<vector> data;

	data_model_base()
	{
		version = default_version;
		comment = default_comment;
		count = 0;
		data.resize(_data_content_count());
	}

	data_model_base(const data_model_base& d) noexcept : count(d.count)
	{
		*this = d;
	}

	data_model_base(data_model_base&& d) noexcept : count(d.count)
	{
		*this = std::move(d);
	}

	virtual ~data_model_base() = default;

	string get_content_name(size_type idx)
	{
		return _data_content_name()[idx];
	}

	virtual size_type get_name_idx(const string& name) const
	{
		return 0;
	}

	data_model_base& operator=(const data_model_base& d) = default;

	data_model_base& operator=(data_model_base&& d) noexcept
	{
		version = std::move(d.version);
		name = std::move(d.name);
		comment = std::move(d.comment);
		count = d.count;
		data = std::move(d.data);
		return *this;
	}

	virtual const vector& get_item(size_type id) const
	{
		return data[id];
	}

	virtual const vector& get_item(const string& name) const
	{
		return data[get_name_idx(name)];
	}

	virtual const vector& get_item_s(const string& name) const
	{
		return get_item(name);
	}

	virtual vector& operator[](size_type id)
	{
		return data[id];
	}

	virtual vector& operator[](const string& name)
	{
		return data[get_name_idx(name)];
	}

	virtual void set_item_s(const string& idx, const vector& p)
	{
		data[get_name_idx(idx)] = p;
	}

	virtual void pop_back()
	{
		for (auto& item : data)
		{
			item.pop_back();
		}
	}

	virtual void resize(size_type size)
	{
		count = size;
		for (auto& item : data)
		{
			item.resize(size);
		}
		for (size_type i = 0; i < size; ++i)
		{
			(*this)[index_name][i] = i + 1;
		}
	}

	virtual size_type size() const
	{
		return data[0].size();
	}

	virtual void load_from_file(const string& path) final
	{
		std::ifstream input_file;
		json j;
		try
		{
			input_file.open(path, std::ifstream::in);
			if (!input_file)
			{
				throw std::runtime_error(
					global::msg("file ", path, "does not exist"));
			}
		}
		catch (std::exception& e)
		{
			global::err(e.what());
			global::err("fail to open file ", path);
			return;
		}
		try
		{
			input_file >> j;
			input_file.close();
		}
		catch (std::exception& e)
		{
			global::err(e.what());
			global::err("fail to load JSON ", path);
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
			output_file.open(path, std::ofstream::out);
		}
		catch (std::exception& e)
		{
			global::err(e.what());
			global::err("fail to open file ", path);
			return;
		}
		try
		{
			output_file << j.dump(4);
			output_file.close();
		}
		catch (std::exception& e)
		{
			global::err(e.what());
			global::err("fail to write file ", path);
		}
	}

	virtual void load_from_json(const json& j)
	{
		load_version(j);
		load_name(j);
		load_comment(j);
		load_count(j);
		load_data(j);

		load_additional_data(j);
	}

	virtual void save_to_json(json& j)
	{
		j = save_to_json();
	}

	virtual json save_to_json()
	{
		json j;
		j[_version()] = version;
		j[_name()] = name;
		j[_comment()] = comment;
		j[_count()] = count;

		json data_j;
		auto names = _data_content_name();
		for (size_type idx = 0; idx < size(); ++idx)
		{
			json unit_j;
			for (size_type i = 0; i < _data_content_count(); ++i)
			{
				unit_j[names[i]] = data[i][idx];
			}
			data_j.emplace_back(unit_j);
		}
		j[_data()] = std::move(data_j);

		json patch_j = save_additional_data();
		if (!patch_j.is_null())
		{
			j.merge_patch(patch_j);
		}
		return j;
	}
};
