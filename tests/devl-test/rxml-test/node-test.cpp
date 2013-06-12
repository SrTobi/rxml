#include "test_settings.hpp"
#include "test_config.hpp"

#include "rxml/value.hpp"
#include "rxml/iterators.hpp"
#include "rxml/locate.hpp"
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

	//#########################################################################################
	void test_locate(const std::string& path, const std::string& expected)
	{
		auto& entity = rxml::get(doc, path);
		const std::string loc = rxml::locate(entity);

		BOOST_CHECK_EQUAL(loc, expected);
	}

	//#########################################################################################
	void test_valuefb(const std::string& path, const std::string& fallback, const std::string& expected)
	{
		std::string result = rxml::valuefb(doc, path, fallback);

		BOOST_CHECK_EQUAL(result, expected);
	}


	rapidxml::xml_document<> doc;
	rapidxml::file<> file;

private:
	//#########################################################################################
	// static tests
	template<typename Iter>
	void static_bidirectional_iterator_test()
	{
		Iter it;
		Iter it1(doc);
		Iter it2(&doc);
		++it;	it++;
		--it;	it--;
		it == it; it != it;
		*it; it->parent();
		it = it;
		it = std::move(it);
	}

	void static_test_iterators()
	{
		static_bidirectional_iterator_test<rxml::node_iterator<>>();
		static_bidirectional_iterator_test<rxml::const_node_iterator<>>();

		static_bidirectional_iterator_test<rxml::attribute_iterator<>>();
		static_bidirectional_iterator_test<rxml::const_attribute_iterator<>>();

		// test range based
		for(auto& test : rxml::children(doc));
		for(auto& test : rxml::children(&doc));
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


	RXML_FIXTURE_TEST(test_locate, "node-test/info", "/node-test/info");
#ifndef _RXML_TYPE_CHECK_HACK
	RXML_FIXTURE_TEST(test_locate, "node-test/info:alt", "/node-test/info:alt");
#else
	RXML_FIXTURE_TEST(test_locate, "node-test/info:alt", "/node-test/info/alt");
#endif

	RXML_FIXTURE_TEST(test_valuefb, "node-test/info:alt", "---", "1");
	RXML_FIXTURE_TEST(test_valuefb, "node-test/inf", "---", "---");
	RXML_FIXTURE_TEST(test_valuefb, "node-test/info/author/..", "---", "Test Info");
	RXML_FIXTURE_TEST(test_valuefb, "node-test/info/author/...", "---", "---");

RXML_END_FIXTURE_TEST()
