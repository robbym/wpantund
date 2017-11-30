#include "DBusIntrospection.h"

Node::Node() {}
Node::Node(const char *name) : name(name) {}
Node& Node::add(Node node)
{
	this->nodes.insert(make_pair(node.name, node));
	return *this;
}
Node& Node::add(Interface iface)
{
	this->ifaces[iface.name] = iface;
	return *this;
}
std::string Node::introspect(const char *path)
{
	std::string _path(path);
	if (_path.length() > 0 && _path[0] == '/') {
		_path = _path.substr(1);
	}
	size_t res = _path.find("/");
	if (res != std::string::npos) {
		std::string subpath = _path.substr(0, res);
		std::map<std::string, Node>::iterator it = this->nodes.find(subpath);
		if (it == this->nodes.end()) {
			throw;
		}
		else
		{
			return it->second.introspect(_path.substr(res+1).c_str());
		}
	}
	else
	{
		std::map<std::string, Node>::iterator it = this->nodes.find(_path);
		if (it != this->nodes.end()) {
			return it->second.render();
		}
		else {
			return this->render();
		}
	}
}
std::string Node::render()
{
	std::string result;

	result += "<node name=\"";
	result += this->name;
	result += "\">";

	for (std::map<std::string, Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		result += it->second.render();
	}

	for (std::map<std::string, Interface>::iterator it = this->ifaces.begin(); it != this->ifaces.end(); ++it) {
		result += it->second.render();
	}

	result += "</node>";

	return result;
}




Interface::Interface() {}
Interface::Interface(const char *name) : name(std::string(name)) {}
std::string Interface::render()
{
	std::string result;

	result += "<interface name=\"";
	result += this->name;
	result += "\">";

	for (std::vector<Method>::iterator it = this->methods.begin(); it != this->methods.end(); ++it) {
		result += it->render();
	}

	result += "</interface>";

	return result;
}
Interface& Interface::add(Method method)
{
	this->methods.push_back(method);
	return *this;
}




Method::Method(const char *name) : name(std::string(name)) {}
std::string Method::render()
{
	std::string result;

	result += "<method name=\"";
	result += this->name;
	result += "\">";

	for (std::vector<Arg>::iterator it = this->args.begin(); it != this->args.end(); ++it) {
		result += it->render();
	}

	result += "</method>";

	return result;
}
Method& Method::add(Arg arg)
{
	this->args.push_back(arg);
	return *this;
}



Arg::Arg(const char *name, const char *type, const char *direction) : name(name), type(type), direction(direction) {}
std::string Arg::render()
{
	std::string result;

	result += "<arg name=\"";
	result += this->name;
	result += "\" type=\"";
	result += this->type;
	result += "\" direction=\"";
	result += this->direction;
	result += "\"/>";

	return result;
}