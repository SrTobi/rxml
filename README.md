rxml
====

Offers some functions to work with rapidxml.

For example you can access values of nodes and attributes by paths:

    rapidxml::xml_document<> doc;
	...
	// load xml file
	...
	std::string window_title = rxml::value(doc, "game/options/graphic/window:title"); // title is an attribute of the window xml-node