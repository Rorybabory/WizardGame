#ifndef PARSER_H
#define PARSER_H
#include "mesh.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <libxml2/libxml/xmlreader.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <iterator>
#include <boost/algorithm/string.hpp>

template <class T>
struct vec {
  operator glm::vec3() const;
  float x,y,z;
};
template <class T>
vec<T>::operator glm::vec3() const {
  glm::vec3 v;
  v.x = x;
  v.y = y;
  v.z = z;
  return v;
}

class Parser {
  public:
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
    std::string vertices;
    std::string normals;
    std::string triangles;
    std::vector<glm::vec3> v;
    std::vector<glm::vec3> n;
    std::vector<unsigned int> tri;
    xmlChar *id = (xmlChar*)"Plane-mesh-normals-array";
    xmlChar *id2 = (xmlChar*)"Plane-mesh-positions-array";
    std::vector<std::string> splitString(std::string split) {
      std::string text = split;
      std::istringstream iss(text);

      std::vector<std::string> results;
      boost::split(results,text, [](char c){return c == ' ';});
      return results;
    }
    Parser(std::string fileName) {
      LIBXML_TEST_VERSION
      char libgeo[] = "library_geometries";
      char src[] = "source";
      char p[] = "p";
      char fa[] = "float_array";
      xmlChar *idFound;
      doc = xmlReadFile(fileName.c_str(),NULL,0);
      if (doc == NULL) {
        printf("error could not parse file");
      }
      root_element = xmlDocGetRootElement(doc);
      xmlNode *nodeFound;
      nodeFound = searchNode(root_element,libgeo);
      nodeFound = searchNode(nodeFound,src);
      if (nodeFound != NULL) {
        nodeFound = searchNodeID(nodeFound,fa, id2);
      }
      if (nodeFound != NULL) {
        vertices = parseNode(doc,nodeFound);
      }

      xmlNode *nodeFound2;
      nodeFound2 = searchNode(root_element,libgeo);
      nodeFound2 = searchNode(nodeFound2,src);
      if (nodeFound2 != NULL) {
        nodeFound2 = searchNodeID(nodeFound2,fa, id);
      }
      if (nodeFound2 != NULL) {
        normals = parseNode(doc,nodeFound2);
      }

      xmlNode *nodeFound3;
      nodeFound3 = searchNode(root_element,libgeo);
      nodeFound3 = searchNode(nodeFound3,src);
      if (nodeFound3 != NULL) {
        nodeFound3 = searchNode(nodeFound3,p);
      }
      if (nodeFound3 != NULL) {
        triangles = parseNode(doc,nodeFound3);
      }
      //print_element_names(root_element,-1);

      vector<std::string> vertS = splitString(vertices);
      vector<std::string> normS = splitString(normals);
      vector<std::string> triS = splitString(triangles);
      vector<float> vertF;
      vector<float> normF;
      vector<float> triF;
      int counterTri = 0;
      int counterVert = 0;
      int counterNorm = 0;
      // std::cout << "vertices: " << vertices << '\n';
      // std::cout << "normals: " << normals << '\n';
      // std::cout << "triangles: " << triangles << '\n';
      for (std::string vert: vertS) {
        float temp = (float)std::stof(vert);
        vertF.push_back(temp);
        std::cout << "vertices:" << temp << '\n';
      }
      for (std::string norm: normS) {
        float temp = (float)std::stof(norm);
        normF.push_back(temp);
        std::cout<< "normals:" << temp << '\n';
      }
      int count = 0;
      for (std::string tri: triS) {
        float temp = (float)std::stof(tri);
        //std::cout<< count << '\n';

        if (count == 0) {
          triF.push_back(temp);
          //std::cout<< "triangles:" << temp << '\n';
          count++;
        }else if (count == 1) {
          count = 0;
        }
      }
      for (float tri: triF) {
        std::cout << "triangles:" << tri << '\n';
      }
      std::cout << '\n' << '\n';
      std::cout << '\n' << '\n';
      vector<vector<float>> vertFSplit;
      vector<vector<float>> normFSplit;
      vector<vector<int>> triISplit;
      for (int i2 = 0; i2<vertF.size()/3; i2++) {
        vector<float> temp;
        for (int i = 0; i<=2; i++) {
          temp.push_back(vertF[counterVert]);
          counterVert++;
        }
        vertFSplit.push_back(temp);
      }

      for (int i2 = 0; i2<normF.size()/3; i2++) {
        vector<float> temp;
        for (int i = 0; i<=2; i++) {
          temp.push_back(normF[counterNorm]);
          counterNorm++;
        }
        normFSplit.push_back(temp);

      }
      std::cout << "SIZE: " << triF.size() << '\n';

      for (int i2 = 0; i2<triF.size()/3; i2++) {
        vector<int> temp;
        for (int i = 0; i<=2; i++) {

          temp.push_back((int)triF[counterTri]);
          std::cout << "Pushed Back: " << triF[counterTri] << " at index: " << counterTri << '\n';
          std::cout << "i2: " << i2 << '\n';
          std::cout << "i: " << i << '\n';
          std::cout << '\n';
          counterTri++;
        }
        triISplit.push_back(temp);
      }


      std::cout << "VERTICES:" << '\n';
      for (std::vector<float> a: vertFSplit) {
        for (float b: a) {
          std::cout << b << " ";
        }
        float* arr = &a[0];
        glm::vec3 tmp = glm::make_vec3(arr);
        v.push_back(tmp);
        std::cout << '\n';
      }
      std::cout << "NORMALS:" << '\n';
      for (std::vector<float> a: normFSplit) {
        for (float b: a) {
          std::cout << b << " ";
        }
        float* arr = &a[0];
        glm::vec3 tmp = glm::make_vec3(arr);
        n.push_back(tmp);
        std::cout << '\n';
      }
      std::cout << "TRIANGLES:" << '\n';
      for (std::vector<int> a: triISplit) {
        for (float b: a) {
          std::cout << b << " ";
        }
        int* arr = &a[0];

        glm::vec3 tmp = glm::make_vec3(arr);
        // t.push_back(tmp);
        std::cout << '\n';
      }
      for (float f: triF) {
        tri.push_back((unsigned int)f);
      }


      xmlFreeDoc(doc);
      xmlCleanupParser();
    }
    xmlNode *searchNodeID(xmlNode *a_node, char target[],xmlChar *id) {
      xmlNode *nodeFound = NULL;
      xmlChar *idFound;
      for (xmlNode *cur = a_node; cur; cur=cur->next) {
        if (cur->type ==XML_ELEMENT_NODE) {
          idFound = xmlGetProp(cur,(const xmlChar*)"id");
          if (!xmlStrcmp(cur->name, (const xmlChar*)target) &&
              (std::string)((char *)idFound)==(std::string)((char *)id)) {
            std::cout << "Found " << cur->name << " with " << id << '\n';
            nodeFound = cur;
            break;
          }else {
            std::cout << "No the id is " << cur->name << '\n';
          }
        }
        if (nodeFound == NULL && cur != NULL) {
          nodeFound = searchNodeID(cur->children, target,id);
        }
      }
      //std::cout << "Found node" << '\n';
      return nodeFound;
    }
    xmlNode *searchNode(xmlNode *a_node, char target[]) {
      xmlNode *nodeFound = NULL;
      for (xmlNode *cur = a_node; cur; cur=cur->next) {
        if (cur->type ==XML_ELEMENT_NODE) {
          if (!xmlStrcmp(cur->name, (const xmlChar*)target)) {
            std::cout << "Found " << cur->name << '\n';
            nodeFound = cur;
            break;
          }
        }
        if (nodeFound == NULL && cur != NULL) {
          nodeFound = searchNode(cur->children, target);
        }
      }
      return nodeFound;
    }
    std::string parseNode(xmlDocPtr doc, xmlNodePtr cur) {
      xmlChar *key;
      cur = cur->xmlChildrenNode;
      while (cur != NULL) {
        key = xmlNodeListGetString(doc,cur, 1);
        return (char *) key;
        xmlFree(key);
        cur = cur->next;
      }
    }
    void print_element_names(xmlNode *root, int level) {
      xmlNode *cur_node = NULL;
      ++level;
      for (cur_node = root; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
          for (int i = 0; i<level; i++) {
            printf(" +");
          }
          printf("%s\n",cur_node->name);
        }
        print_element_names(cur_node->children, level);
      }
    }
  protected:

  private:
};
#endif
