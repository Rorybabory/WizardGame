#ifndef MESSENGING_H
#define MESSENGING_H
#include <vector>
#include <string>
#include <algorithm>
class Message {
public:
  Message(std::string target, std::string text) {
    this->target = target;
    this->text = text;
  }
  Message(std::string target, glm::vec2 Vector2D) {
    this->target = target;
    this->Vector2D = Vector2D;
  }
  Message(std::string target, glm::vec2 Vector2D, std::string text) {
    this->target = target;
    this->text = text;
    this->Vector2D = Vector2D;
  }
  Message(std::string target, glm::vec2 Vector2D, float Float, std::string text) {
    this->target = target;
    this->text = text;
    this->Vector2D = Vector2D;
    this->Float = Float;
  }
  Message(std::string target, glm::vec2 Vector2D, glm::vec3 Vector3D, std::string text) {
    this->target = target;
    this->text = text;
    this->Vector2D = Vector2D;
    this->Vector3D = Vector3D;
  }
  Message(std::string target, float Float) {
    this->target = target;
    this->Float = Float;
  }
  Message(std::string target, std::vector<glm::vec2> Vec2Vector) {
    this->target = target;
    this->Vec2Vector = Vec2Vector;
  }
  Message(std::string target, std::vector<glm::vec2> Vec2Vector, std::vector<std::string> StringVector) {
    this->target = target;
    this->Vec2Vector = Vec2Vector;
    this->StringVector = StringVector;
    this->text = text;
  }

  Message(std::string target, std::vector<glm::vec3> Vec3Vector, std::vector<glm::vec2> Vec2Vector) {
    this->target = target;
    this->Vec2Vector = Vec2Vector;
    this->Vec3Vector = Vec3Vector;
  }
  Message() {}

  std::string target;
  std::string text;
  bool boolean;
  glm::vec2 Vector2D;
  glm::vec3 Vector3D;
  std::vector<glm::vec2> Vec2Vector;
  std::vector<glm::vec3> Vec3Vector;
  std::vector<bool> BoolVector;
  std::vector<std::string> StringVector;
  float Float;
};
class Messager {
public:
  Messager() {}
  void clearBus() {
    recievedMessage = Message("","");
    sentMessage = Message("","");
  }
  Message getMessage() {
    return recievedMessage;
  }
  Message getSentMessage() {
    return sentMessage;
  }
  void recieveMessage(Message message) {
    allMessages.push_back(message);
    recievedMessage = message;
  }
  void sendMessage(Message message) {
    allMessages.push_back(message);
    sentMessage = message;
  }
private:
  std::vector<Message> allMessages;
  Message sentMessage;
  Message recievedMessage;
};
#endif
