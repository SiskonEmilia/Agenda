#include "Date.hpp"
#include <cstdio>
#include <iostream>
/*
#ifndef DATE_H
#define DATE_H

#include <initializer_list>
#include <string>

class Date {
 public:
  /**
  * @brief default constructor
  
  Date();

  /**
  * @brief constructor with arguments
  
  Date(int t_year, int t_month, int t_day, int t_hour, int t_minute);

  /**
  * @brief constructor with a string
  
  Date(const std::string &dateString);
  /**
  * @brief return the year of a Date
  * @return   a integer indicate the year of a date
  
  int getYear(void) const;

  /**
  * @brief set the year of a date
  * @param a integer indicate the new year of a date
  
  void setYear(const int t_year);

  /**
  * @brief return the month of a Date
  * @return   a integer indicate the month of a date
  
  int getMonth(void) const;

  /**
  * @brief set the month of a date
  * @param a integer indicate the new month of a date
  
  void setMonth(const int t_month);

  /**
  * @brief return the day of a Date
  * @return   a integer indicate the day of a date
  
  int getDay(void) const;

  /**
  * @brief set the day of a date
  * @param a integer indicate the new day of a date
  
  void setDay(const int t_day);

  /**
  * @brief return the hour of a Date
  * @return   a integer indicate the hour of a date
  
  int getHour(void) const;

  /**
  * @brief set the hour of a date
  * @param a integer indicate the new hour of a date
  
  void setHour(const int t_hour);

  /**
  * @brief return the minute of a Date
  * @return   a integer indicate the minute of a date
  
  int getMinute(void) const;

  /**
  * @brief set the minute of a date
  * @param a integer indicate the new minute of a date
  
  void setMinute(const int t_minute);

  /**
  *   @brief check whether the date is valid or not
  *   @return the bool indicate valid or not
  
  static bool isValid(const Date &t_date);

  /**
  * @brief convert a string to date, if the format is not correct return
  * 0000-00-00/00:00
  * @return a date
  
  static Date stringToDate(const std::string &t_dateString);

  /**
  * @brief convert a date to string, if the format is not correct return
  * 0000-00-00/00:00
  
  static std::string dateToString(const Date &t_date);

  /**
  *  @brief overload the assign operator
  
  Date &operator=(const Date &t_date);

  /**
  * @brief check whether the CurrentDate is equal to the t_date
  
  bool operator==(const Date &t_date) const;

  /**
  * @brief check whether the CurrentDate is  greater than the t_date
  
  bool operator>(const Date &t_date) const;

  /**
  * @brief check whether the CurrentDate is  less than the t_date
  
  bool operator<(const Date &t_date) const;

  /**
  * @brief check whether the CurrentDate is  greater or equal than the t_date
  
  bool operator>=(const Date &t_date) const;

  /**
  * @brief check whether the CurrentDate is  less than or equal to the t_date
  
  bool operator<=(const Date &t_date) const;

 private:
  int m_year;
  int m_month;
  int m_day;
  int m_hour;
  int m_minute;
};

#endif
*/
#include <sstream>

using namespace std;

Date::Date()
{
 m_year = 0;
 m_month = 0;
 m_day = 0;
 m_hour = 0;
 m_minute = 0;
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute)
{
 m_year = t_year;
 m_month = t_month;
 m_day = t_day;
 m_hour = t_hour;
 m_minute = t_minute;
}

Date::Date(const std::string &dateString)
{
 *this = stringToDate(dateString);
}

int Date::getYear(void) const{
 return m_year;
}

void Date::setYear(const int t_year){
 m_year = t_year;
}

int Date::getMonth(void) const{
 return m_month;
}

void Date::setMonth(const int t_month){
 m_month = t_month;
}

int Date::getDay(void) const{
 return m_day;
}

void Date::setDay(const int t_day){
 m_day = t_day;
}

int Date::getHour(void) const{
 return m_hour;
}

void Date::setHour(const int t_hour){
 m_hour = t_hour;
}

int Date::getMinute(void) const{
 return m_minute;
}

void Date::setMinute(const int t_minute){
 m_minute = t_minute;
}

bool Date::isValid(const Date &t_date)
{
 int year = t_date.m_year,
  month = t_date.m_month,
  day = t_date.m_day,
  hour = t_date.m_hour,
  minute = t_date.m_minute;
 int monthDay[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
 if(year < 1000 || year > 9999)
  return false;
 if(month < 1 || month > 12)
  return false;
 if(day < 1 || day > 31)
  return false;
 if(hour < 0 || hour > 23)
  return false;
 if(minute < 0 || minute > 59)
  return false;
 if(day > monthDay[month])
 {
  if(month != 2)
   return false;
  else if(!(year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)))
   return false;
  else if (day > 29)
   return false;
 }
 return true;
}

Date Date::stringToDate(const string &t_dateString)
{
 Date emptyClass;
 if(t_dateString.size() != 16)
  return emptyClass;

 if(t_dateString[4] != '-' || t_dateString[7] != '-' || 
  t_dateString[10] != '/' || t_dateString[13] != ':')
  return emptyClass;

 stringstream temp, toConv;
 string toJudge;
 temp << t_dateString;
 int catcher;
 Date res;

 if(!getline(temp, toJudge, '-'))
  return emptyClass;
 if(toJudge.size() != 4)
  return emptyClass;
 toConv.clear();
 toConv << toJudge;
 toConv >> catcher;
 res.setYear(catcher);

 if(!getline(temp, toJudge, '-'))
  return emptyClass;
 if(toJudge.size() != 2)
  return emptyClass;;
 toConv.clear();
 toConv << toJudge;
 toConv >> catcher;
 res.setMonth(catcher);

 if(!getline(temp, toJudge, '/'))
  return emptyClass;;
 if(toJudge.size() != 2)
  return emptyClass;;
 toConv.clear();
 toConv << toJudge;
 toConv >> catcher;
 res.setDay(catcher);

 if(!getline(temp, toJudge, ':'))
  return emptyClass;;
 if(toJudge.size() != 2)
  return emptyClass;;
 toConv.clear();
 toConv << toJudge;
 toConv >> catcher;
 res.setHour(catcher);

 temp >> toJudge;
 if(toJudge.size() != 2)
  return emptyClass;;
 toConv.clear();
 toConv << toJudge;
 toConv >> catcher;
 res.setMinute(catcher);

 if(!isValid(res))
  return emptyClass;

 return res;
}

string Date::dateToString(const Date &t_date)
{
 if(!Date::isValid(t_date))
  return "0000-00-00/00:00";
 string res;
 char temp[17];
 sprintf(temp, "%04d-%02d-%02d/%02d:%02d", t_date.m_year,
  t_date.m_month, t_date.m_day, t_date.m_hour, t_date.m_minute);
 res = temp;
 return res;
}

Date& Date::operator=(const Date &t_date)
{
 m_year = t_date.m_year;
 m_month = t_date.m_month;
 m_day = t_date.m_day;
 m_hour = t_date.m_hour;
 m_minute = t_date.m_minute;
 return *this;
}

bool Date::operator==(const Date &t_date) const
{
 if(m_year != t_date.m_year)
  return false;
 if(m_month != t_date.m_month)
  return false;
 if(m_day != t_date.m_day)
  return false;
 if(m_hour != t_date.m_hour)
  return false;
 if(m_day != t_date.m_day)
  return false;
 return true;
}

bool Date::operator>(const Date &t_date) const
{
 return (dateToString(*this) > dateToString(t_date));
}

bool Date::operator<(const Date &t_date) const
{
 return (dateToString(*this) < dateToString(t_date));
}

/**
* @brief check whether the CurrentDate is  greater or equal than the t_date
*/
bool Date::operator>=(const Date &t_date) const
{
 return (dateToString(*this) >= dateToString(t_date));
}

/**
* @brief check whether the CurrentDate is  less than or equal to the t_date
*/
bool Date::operator<=(const Date &t_date) const
{
 return (dateToString(*this) <= dateToString(t_date));
}