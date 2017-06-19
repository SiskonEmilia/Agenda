#include "Storage.hpp"
#include "Path.hpp"
#include <fstream>

using namespace std;

shared_ptr<Storage> Storage::m_instance = nullptr;

shared_ptr<Storage> Storage::getInstance(void)
{
 if(m_instance == nullptr)
  m_instance = shared_ptr<Storage>(new Storage);

 return m_instance;
}

Storage::Storage()
{
 m_dirty = false;
 readFromFile();
}


bool Storage::readFromFile(void)
{
 myCoder();
 ifstream userFile(Path::userPath);
 ifstream meetingFile(Path::meetingPath);
 
 if(!(userFile.is_open() && meetingFile.is_open()))
  return false;

 char rubbish = 0;
 string strIn;
 User toPush;

 while(userFile >> rubbish)
 {
  getline(userFile, strIn, '"');
  toPush.setName(strIn);
  userFile >> rubbish >> rubbish;

  getline(userFile, strIn, '"');
  toPush.setPassword(strIn);
  userFile >> rubbish >> rubbish;

  getline(userFile, strIn, '"');
  toPush.setEmail(strIn);
  userFile >> rubbish >> rubbish;

  getline(userFile, strIn, '"');
  toPush.setPhone(strIn);

  m_userList.push_back(toPush);
 }

 string temp;
 Meeting toAdd;
 vector<string> emptyVector;
 int index = 0;
 while(meetingFile >> rubbish)
 {
  getline(meetingFile, strIn, '"');
  toAdd.setSponsor(strIn);
  meetingFile >> rubbish >> rubbish;

  getline(meetingFile, strIn, '"');
  toAdd.setParticipator(emptyVector);
  while((index = strIn.find('&')) != -1)
  {
   temp = strIn.substr(0, index);
   strIn.erase(0, index + 1);
   toAdd.addParticipator(temp);
  }
  if(strIn.size() != 0)
  {
   toAdd.addParticipator(strIn);
  }
  meetingFile >> rubbish >> rubbish;

  getline(meetingFile, strIn, '"');
  toAdd.setStartDate(strIn);
  meetingFile >> rubbish >> rubbish;

  getline(meetingFile, strIn, '"');
  toAdd.setEndDate(strIn);
  meetingFile >> rubbish >> rubbish;

  getline(meetingFile, strIn, '"');
  toAdd.setTitle(strIn);

  m_meetingList.push_back(toAdd);
 }

 userFile.close();
 meetingFile.close();

 return true;
}

bool Storage::writeToFile(void)
{
 ofstream userFile(Path::userPath), meetingFile(Path::meetingPath);



 for(auto c: m_userList)
 {
  userFile << "\"" << c.getName() << "\","
   << "\"" << c.getPassword() << "\","
   << "\"" << c.getEmail() << "\","
   << "\"" << c.getPhone() << "\"\n";
 }

 vector<string> emptyVector;
 int counter = 0;
 for(auto c: m_meetingList)
 {
  meetingFile << "\"" << c.getSponsor() << "\",";
  
  emptyVector = c.getParticipator();
  if(emptyVector.empty())
   meetingFile << "\"\",";
  else
  {
   counter = 0;
   meetingFile << "\"" << emptyVector[0];
   for(auto i: emptyVector)
   {
    if(counter++)
     meetingFile << "&" << i;
   }
   meetingFile << "\",";
  }

  meetingFile << "\"" << Date::dateToString(c.getStartDate()) << "\","
   << "\"" << Date::dateToString(c.getEndDate()) << "\","
   << "\"" << c.getTitle() << "\"\n";
 }

 userFile.close();
 meetingFile.close();

 m_dirty = false;
 myCoder();
 return true;
}

Storage::~Storage()
{
  sync();
}

void Storage::createUser(const User &t_user)
{
 m_userList.push_back(t_user);
 m_dirty = true;
}

list<User> Storage::queryUser(std::function<bool(const User &)> filter) const
{
 list<User> toReturn;
 for(auto e: m_userList)
 {
  if(filter(e))
   toReturn.push_back(e);
 }
 return toReturn;
}

int Storage::updateUser(std::function<bool(const User &)> filter,
                 std::function<void(User &)> switcher)
{
 int counter = 0;
 for(auto &e:m_userList)
 {
  if(filter(e))
  { 
   switcher(e);
   counter++;
   m_dirty = true;
  }
 }
 return counter;
}

int Storage::deleteUser(std::function<bool(const User &)> filter)
{
 int counter = 0;
 for(auto itor = m_userList.begin(); itor != m_userList.end();)
 {
  if(filter(*itor)){
   itor = m_userList.erase(itor);
   counter++;
   m_dirty = true;
  }
  else
   itor++;
 }
 return counter;
}

void Storage::createMeeting(const Meeting &t_meeting)
{
 m_meetingList.push_back(t_meeting);
 m_dirty = true;
}

list<Meeting> Storage::queryMeeting(
      std::function<bool(const Meeting &)> filter) const
{
 list<Meeting> toReturn;
 for(auto e: m_meetingList)
 {
  if(filter(e))
   toReturn.push_back(e);
 }
 return toReturn;
}

int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                    std::function<void(Meeting &)> switcher)
{
 int counter = 0;
 for(auto &e: m_meetingList)
 {
  if(filter(e))
  { 
   switcher(e);
   counter++;
   m_dirty = true;
  }
 }
 return counter;
}

int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter)
{
 int counter = 0;
 for(auto itor = m_meetingList.begin(); itor != m_meetingList.end();)
 {
  if(filter(*itor)){
   itor = m_meetingList.erase(itor);
   counter++;
   m_dirty = true;
  }
  else
   itor++;
 }
 return counter;
}

bool Storage::sync(void)
{
 return writeToFile();
}





