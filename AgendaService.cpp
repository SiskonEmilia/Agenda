#include "AgendaService.hpp"
#include <iostream>
#include <QString>

QString str2qstr(const string str)
{
	return QString::fromLocal8Bit(str.data());
}

string qstr2str(const QString qstr)
{
	QByteArray cdata = qstr.toLocal8Bit();
	return string(cdata);
}
///////////////////////////////////////
//private members:                   //
//std::shared_ptr<Storage> m_storage;//
///////////////////////////////////////
using namespace std;
/**
* constructor
*/
AgendaService::AgendaService(){startAgenda();}

/**
* destructor
*/
AgendaService::~AgendaService(){quitAgenda();}

/**
* check if the username match password
* @param userName the username want to login
* @param password the password user enter
* @return if success, true will be returned
*/
bool AgendaService::userLogIn(const std::string &userName, const std::string &password){
 list<User> toLogin = m_storage->queryUser(
  [userName, password](const User & userIn) -> bool{
   return (userIn.getName() == userName && userIn.getPassword() == password);
  });
 return !toLogin.empty();// question: how to hold present user?
}

/**
* regist a user
* @param userName new user's username
* @param password new user's password
* @param email new user's email
* @param phone new user's phone
* @return if success, true will be returned
*/
bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                const std::string &email, const std::string &phone){
 list<User> toLogin = m_storage->queryUser(
  [userName, password](const User & userIn) -> bool{
   return (userIn.getName() == userName);
  });
 //username check
 if(userName == "")
  return false;
 if(toLogin.empty()){
  User toCreat(userName, password, email, phone);
  m_storage->createUser(toCreat);
  return true;
 }
 else
  return false;
}

/**
* delete a user
* @param userName user's username
* @param password user's password
* @return if success, true will be returned
*/
bool AgendaService::deleteUser(const std::string &userName, const std::string &password){
 if(!m_storage->queryUser(
  [userName, password](const User & userIn) -> bool{
   return (userIn.getName() == userName && userIn.getPassword() == password);
  }).empty())
 {
  list<Meeting> toQuit = listAllSponsorMeetings(userName);
  for(auto i:toQuit){
   deleteMeeting(userName, i.getTitle());
  }// delete all meetings hold by user

  toQuit = listAllMeetings(userName);
  for(auto i: toQuit){
   quitMeeting(userName, i.getTitle());// if none takes part in, delete
  }// remove from participator list
  m_storage->deleteUser(
  [userName, password](const User & userIn) -> bool{
   return (userIn.getName() == userName && userIn.getPassword() == password);
  });
  return true;
 }
 return false;// not an existed user or incorrect password
}

/**
* list all users from storage
* @return a user list result
*/
std::list<User> AgendaService::listAllUsers(void) const{
 return m_storage->queryUser(
  [](const User & userIn) -> bool{return true;});
}

/**
* create a meeting
* @param userName the sponsor's userName
* @param title the meeting's title
* @param participator the meeting's participator
* @param startDate the meeting's start date
* @param endDate the meeting's end date
* @return if success, true will be returned
*/
bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                 const std::string &startDate, const std::string &endDate,
                 const std::vector<std::string> &participator){
//Existence of user check
	if(m_storage->queryUser(
		[&](const User& userIn) -> bool{
			return (userIn.getName() == userName);
		}).empty())
		return false;

  if(participator.empty())
    return false;

	for(auto u: participator){
		if(m_storage->queryUser(
		[&](const User& userIn) -> bool{
			return (userIn.getName() == u);
		}).empty())
		return false;
	}

//Repeatation check
  for(auto i = participator.begin(), j = participator.end(); i != j; i++){
    if(*i == userName)
      return false;
    for(auto t = i + 1; t != j; t++)
      if(*t == *i)
        return false;
  }

//Time vaild check
	if(!(Date::isValid(startDate) && Date::isValid(endDate)))
		return false;

	if(endDate <= startDate)
		return false;

//Time argument check
	//Sponsor check
	list<Meeting> toCheck;
	toCheck = listAllMeetings(userName);
	for(auto i: toCheck){
		if(i.getStartDate() <= startDate && i.getEndDate() > startDate)
			return false;
		if(i.getStartDate() < endDate && i.getEndDate() >= endDate)
			return false;
    if(i.getStartDate() >= startDate && i.getEndDate() <= endDate)
      return false;
	}

	//Participator check
	for(auto t: participator){
		toCheck = listAllMeetings(t);
		for(auto i: toCheck){
			if(i.getStartDate() <= startDate && i.getEndDate() > startDate)
				return false;
			if(i.getStartDate() < endDate && i.getEndDate() >= endDate)
				return false;
      if(i.getStartDate() >= startDate && i.getEndDate() <= endDate)
      return false;
		}
	}

//Title argument check
	if(!m_storage->queryMeeting(
		[&](const Meeting& meetingIn) -> bool{
			return (meetingIn.getTitle() == title);
		}).empty())
		return false;

//All check passed, creat the meeting
	Meeting toCreat(userName, participator, startDate, endDate, title);
	m_storage->createMeeting(toCreat);

	return true;
}

/**
* add a participator to a meeting
* @param userName the sponsor's userName
* @param title the meeting's title
* @param participator the meeting's participator
* @return if success, true will be returned
*/
bool AgendaService::addMeetingParticipator(const std::string &userName,
                          const std::string &title,
                          const std::string &participator){
 if(m_storage->queryUser([userName](const User& userIn){return (userIn.getName() == userName);}).empty())
  return false;
 if(m_storage->queryUser([&](const User& userIn){return (userIn.getName() == participator);}).empty())
  return false;
 list<Meeting> toAdd = meetingQuery(userName, title);
 if(toAdd.empty())
  return false;
 if(toAdd.front().isParticipator(participator))
  return false;

 Date stime = toAdd.front().getStartDate(),
  etime = toAdd.front().getEndDate();

 if(!m_storage->queryMeeting(
  [&](const Meeting & meetingIn) -> bool{
   bool res = (meetingIn.isParticipator(participator) ||
    meetingIn.getSponsor() == participator);
   res &= ((meetingIn.getStartDate() < etime && meetingIn.getStartDate() >= stime) ||
    (meetingIn.getEndDate() <= etime && meetingIn.getEndDate() > stime) ||
    (stime >= meetingIn.getStartDate() && etime <= meetingIn.getEndDate()));
   return res;
  }).empty())
 {
  return false;
 }
 return m_storage->updateMeeting(
  [title, userName](const Meeting &meetingIn) -> bool{
   return (meetingIn.getTitle() == title && meetingIn.getSponsor() == userName);
  },
  [participator](Meeting & meetingIn) -> void{
   meetingIn.addParticipator(participator);
  });
}

/**
* remove a participator from a meeting
* @param userName the sponsor's userName
* @param title the meeting's title
* @param participator the meeting's participator
* @return if success, true will be returned
*/
bool AgendaService::removeMeetingParticipator(const std::string &userName,
                             const std::string &title,
                             const std::string &participator){
 if(m_storage->queryUser([userName](const User& userIn){return (userIn.getName() == userName);}).empty())
  return false;

 bool isRemove = m_storage->updateMeeting(
  [&](const Meeting &meetingIn) -> bool{
   return (meetingIn.getTitle() == title && 
   	meetingIn.getSponsor() == userName &&
   	meetingIn.isParticipator(participator));
  },
  [participator](Meeting & meetingIn) -> void{
   meetingIn.removeParticipator(participator);
  });

 if(isRemove){
  if(m_storage->queryMeeting(
   [title](const Meeting &meetingIn) -> bool{
    return (meetingIn.getTitle() == title);
   }).front().getParticipator().empty()){
   deleteMeeting(userName, title);
  }
 }

 return isRemove;
}

/**
* quit from a meeting
* @param userName the current userName. no need to be the sponsor
* @param title the meeting's title
* @return if success, true will be returned
*/
bool AgendaService::quitMeeting(const std::string &userName, const std::string &title){
 if(m_storage->queryUser([userName](const User& userIn){return (userIn.getName() == userName);}).empty())
  return false;

  bool isRemove = m_storage->updateMeeting(
  [title, userName](const Meeting &meetingIn) -> bool{
   return (meetingIn.getTitle() == title &&
   	meetingIn.isParticipator(userName));
  },
  [userName](Meeting & meetingIn) -> void{
   meetingIn.removeParticipator(userName);
  });
  if(isRemove){
   if(m_storage->queryMeeting(
   [title](const Meeting &meetingIn) -> bool{
    return (meetingIn.getTitle() == title);
   }).front().getParticipator().empty()){
    m_storage->deleteMeeting(
     [title](const Meeting & meetingIn) -> bool{
      return (meetingIn.getTitle() == title);
     });
   }
	}
  return isRemove;
}

/**
* search a meeting by username and title
* @param uesrName the sponsor's userName
* @param title the meeting's title
* @return a meeting list result
*/
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                              const std::string &title) const{
 return m_storage->queryMeeting(
  [userName, title](const Meeting & meetingIn) -> bool{
   return(meetingIn.getTitle() == title && (
    meetingIn.getSponsor() == userName ||
    meetingIn.isParticipator(userName)));
  });
}
/**
* search a meeting by username, time interval
* @param uesrName the sponsor's userName
* @param startDate time interval's start date
* @param endDate time interval's end date
* @return a meeting list result
*/
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                              const std::string &startDate,
                              const std::string &endDate) const{
 list<Meeting> emptyList;
 if(m_storage->queryUser([userName](const User& userIn){return (userIn.getName() == userName);}).empty())
  return emptyList;
 if(!(Date::isValid(startDate) && Date::isValid(endDate)))
  return emptyList;
 if(startDate > endDate)
  return emptyList;
 return m_storage->queryMeeting(
  [userName, startDate, endDate](const Meeting & meetingIn) -> bool{
   return((meetingIn.getSponsor() == userName || 
    meetingIn.isParticipator(userName)) && 
    ((meetingIn.getEndDate() <= endDate && 
     meetingIn.getEndDate() >= startDate) ||
    (meetingIn.getStartDate() <= endDate && 
     meetingIn.getStartDate() >= startDate) ||
    (meetingIn.getStartDate() <= startDate &&
      meetingIn.getEndDate() >= endDate)));
  });
}

/**
* list all meetings the user take part in
* @param userName user's username
* @return a meeting list result
*/
std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const{
 return m_storage->queryMeeting(
  [userName](const Meeting & meetingIn) -> bool{
   return (meetingIn.isParticipator(userName) ||
    meetingIn.getSponsor() == userName);
  });
}

/**
* list all meetings the user sponsor
* @param userName user's username
* @return a meeting list result
*/
std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const{
 return m_storage->queryMeeting(
  [userName](const Meeting & meetingIn) -> bool{
   return (meetingIn.getSponsor() == userName);
  });
}

/**
* list all meetings the user take part in and sponsor by other
* @param userName user's username
* @return a meeting list result
*/
std::list<Meeting> AgendaService::listAllParticipateMeetings(
  const std::string &userName) const{
 return m_storage->queryMeeting(
  [userName](const Meeting & meetingIn) -> bool{
   return (meetingIn.isParticipator(userName));
  });
}

/**
* delete a meeting by title and its sponsor
* @param userName sponsor's username
* @param title meeting's title
* @return if success, true will be returned
*/
bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title){
 return m_storage->deleteMeeting([userName, title](const Meeting& meetingIn) -> bool
  {
   return (meetingIn.getTitle() == title
    && meetingIn.getSponsor() == userName);
  });
}

/**
* delete all meetings by sponsor
* @param userName sponsor's username
* @return if success, true will be returned
*/
bool AgendaService::deleteAllMeetings(const std::string &userName){
 return m_storage->deleteMeeting([userName](const Meeting& meetingIn) -> bool
  {
   return (meetingIn.getSponsor() == userName);
  });
}

/**
* start Agenda service and connect to storage
*/
void AgendaService::startAgenda(void){
 m_storage = Storage::getInstance();
}

/**
* quit Agenda service
*/
void AgendaService::quitAgenda(void){}

void AgendaService::saveNow(void) {
	m_storage->sync();
}

AgendaService dataService;
std::string presentUser;
std::string presentPass;