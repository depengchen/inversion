#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#define LOG(msg) global::log(__FUNCTION__, msg)

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
		return std::string(t);
	}

	//����������ϲ�Ϊ�ַ���
	template <typename T, typename ...Args>
	std::string msg(const T& t, const Args&...args)
	{
		std::string ss = t;
		return ss + msg(args...);
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

	using vector=std::vector<float_t>;

	//pi
	constexpr float_t pi = 3.14159265359;
	//mu_0
	constexpr float_t mu0 = 1.25663706e-6;
}
