#pragma once

#include <map>
#include <vector>
#include <string>
#include <map>

class Node;
class Interface;
class Method;
class Arg;

class Node
{
	std::string name;
	std::map<std::string, Node> nodes;
	std::map<std::string, Interface> ifaces;

	std::string render();

public:
	Node();
	Node(const char *name);

	Node& add(Node node);
	Node& add(Interface iface);

	std::string introspect(const char *path);
};

class Interface
{
	std::string name;
	std::vector<Method> methods;

	std::string render();

	friend class Node;

public:
	Interface();
	Interface(const char *name);

	Interface& add(Method method);
};

class Method
{
	std::string name;
	std::vector<Arg> args;

	std::string render();

	friend class Interface;

public:
	Method(const char *name);

	Method& add(Arg arg);
};

class Arg
{
	std::string name;
	std::string type;
	std::string direction;

	std::string render();

	friend class Method;

public:
	Arg(const char *name, const char *type, const char *direction);
};




class IntrospectionCache {
	Node node;
	std::map<std::string, std::string> cache;
	std::map<std::string, const char *> ptr_cache;
public:
	IntrospectionCache(Node node);

	const char **lookup(const char *);
};