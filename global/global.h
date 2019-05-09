#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#define LOG(msg) global::log(__FUNCTION__, msg)
#define TIMER(...) global::scoped_timer __t(__FUNCTION__)

namespace global
{
	namespace detail
	{
		void _log(const std::string& tag, const std::string& content) noexcept;
	}

	std::string current_time();

	//����������ϲ�Ϊ�ַ���
	template <typename T>
	std::string msg(const T& t)
	{
		std::stringstream ss;
		ss << t;
		return ss.str();
	}

	//����������ϲ�Ϊ�ַ���
	template <typename T, typename ...Args>
	std::string msg(const T& t, const Args&...args)
	{
		std::stringstream ss;
		ss << t << msg(args...);
		return ss.str();
	}

	//���������Ϣ��std::cerr
	template <typename...Args>
	void err(const Args& ...args)
	{
		std::cerr << msg(args...) << std::endl;
	}

	//�����־��������ڣ�����T��U��������л�
	template <typename T, typename ...Args>
	void log(const T& tag, const Args& ...content) noexcept
	{
		try
		{
			detail::_log(msg(tag), msg(content...));
		}
		catch (std::exception& e)
		{
			detail::_log("log", e.what());
		}
	}

	//�������ʱ�����˳�������ʱ���ʱ��
	class scoped_timer
	{
	private:
		void* timer;
	public :
		scoped_timer(std::string name);
		~scoped_timer();
	};
}


namespace global
{
	//���������ʹ�õĸ�������
	using float_t = double;

	using vector = std::vector<float_t>;

	//pi
	constexpr float_t pi = 3.14159265359;
	//mu_0
	constexpr float_t mu0 = 4e-7 * pi;
}
