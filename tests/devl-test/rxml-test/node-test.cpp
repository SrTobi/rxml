#include "test_settings.hpp"
#include "test_config.hpp"

#include "rxml/value.hpp"
#include "rxml/iterators.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_iterators.hpp"


namespace fs = boost::filesystem;

struct NodeTestFixture
{
	NodeTestFixture(const fs::path& xml_path)
		: file(xml_path.string().c_str())
	{
		BOOST_REQUIRE(file.size());
		doc.parse<rapidxml::parse_full | rapidxml::parse_trim_whitespace | rapidxml::parse_normalize_whitespace>(file.data());
	}

	//#########################################################################################
	void test_pgetnode_with_has_const(const std::string& path, bool has = true) const
	{
		BOOST_CHECK_EQUAL(rxml::getnode(&doc, path) != nullptr, has);
	}

	void test_pgetnode_with_has(const std::string& path, bool has = true)
	{
		BOOST_CHECK_EQUAL(rxml::getnode(&doc, path) != nullptr, has);
		test_pgetnode_with_has_const(path, has);
	}

	//#########################################################################################
	void test_rgetnode_no_throws_exception_const(const std::string& path) const
	{
		rxml::getnode(doc, path);
	}

	void test_rgetnode_no_throws_exception(const std::string& path)
	{
		rxml::getnode(doc, path);
		test_rgetnode_no_throws_exception_const(path);
	}

	//#########################################################################################
	void test_rgetnode_throws_exception_const(const std::string& path) const
	{
		BOOST_CHECK_THROW(rxml::getnode(doc, path), rxml::notfound_error);
	}


	void test_rgetnode_throws_exception(const std::string& path)
	{
		BOOST_CHECK_THROW(rxml::getnode(doc, path), rxml::notfound_error);
		test_rgetnode_throws_exception_const(path);
	}

	//#########################################################################################
	void test_value_no_throw_const(const std::string& path, const std::string& expected) const
	{
		std::string result = rxml::value(doc, path);
		BOOST_CHECK_EQUAL(result, expected);
	}

	void test_value_no_throw(const std::string& path, const std::string& expected)
	{
		std::string result = rxml::value(doc, path);
		BOOST_CHECK_EQUAL(result, expected);

		test_value_no_throw_const(path, expected);
	}

	//#########################################################################################
	void test_valuex_no_throw_const(const std::string& path, const std::string& regex) const
	{
		rxml::valuex(doc, path, regex);
	}

	void test_valuex_no_throw(const std::string& path, const std::string& regex)
	{
		rxml::valuex(doc, path, regex);

		test_valuex_no_throw_const(path, regex);
	}




	rapidxml::xml_document<> doc;
	rapidxml::file<> file;

private:
	//#########################################################################################
	// static tests
	void static_test_iterator()
	{
		rxml::node_iterator<> it;
		++it;	it++;
		--it;	it--;
		*it; it->last_node();
	}
};




RXML_START_FIXTURE_TEST(NodeTestFixture, get_rxml_test_path() / "node-test-1.xml")

	RXML_FIXTURE_TEST(test_pgetnode_with_has, "");
	RXML_FIXTURE_TEST(test_pgetnode_with_has, ":name");	// name does not exists in root !!!
	RXML_FIXTURE_TEST(test_pgetnode_with_has, "node-test/xxx/sample");
	RXML_FIXTURE_TEST(test_pgetnode_with_has, "node-test/xxx/sample:attr");
	RXML_FIXTURE_TEST(test_pgetnode_with_has, "node-test/xxxx/sample", false);


	RXML_FIXTURE_TEST(test_rgetnode_no_throws_exception, "/");
	RXML_FIXTURE_TEST(test_rgetnode_no_throws_exception, "node-test/info:alt");
	RXML_FIXTURE_TEST(test_rgetnode_no_throws_exception, "node-test/info/../info/author");
	RXML_FIXTURE_TEST(test_rgetnode_no_throws_exception, "/node-test/info/author");
	RXML_FIXTURE_TEST(test_rgetnode_no_throws_exception, "node-test/xxx/sample");


	RXML_FIXTURE_TEST(test_rgetnode_throws_exception, "/..");
	RXML_FIXTURE_TEST(test_rgetnode_throws_exception, "node-test/../..");
	RXML_FIXTURE_TEST(test_rgetnode_throws_exception, "node-test/info/alt");
	RXML_FIXTURE_TEST(test_rgetnode_throws_exception, "node-test/info/VERSION");
	RXML_FIXTURE_TEST(test_rgetnode_throws_exception, "node-test/xxxx/sample");

	RXML_FIXTURE_TEST(test_value_no_throw, "node-test:name", "node-test");
	RXML_FIXTURE_TEST(test_value_no_throw, "node-test/info:alt", "1");
	RXML_FIXTURE_TEST(test_value_no_throw, "node-test/info/author:nick", "SirTobi");
	RXML_FIXTURE_TEST(test_value_no_throw, "node-test/list/value", "hallo");
	RXML_FIXTURE_TEST(test_value_no_throw, "node-test/info", "Test Info");
	RXML_FIXTURE_TEST(test_value_no_throw, "node-test/info/author/..", "Test Info");

	RXML_FIXTURE_TEST(test_valuex_no_throw, "node-test/info:alt", "[[:digit:]]");
	RXML_FIXTURE_TEST(test_valuex_no_throw, "node-test/info", "[a-zA-Z ]+");

RXML_END_FIXTURE_TEST()