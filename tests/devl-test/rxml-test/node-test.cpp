#include "test_settings.hpp"
#include "test_config.hpp"

#include "rxml/value.hpp"
#include "rapidxml_utils.hpp"


namespace fs = boost::filesystem;

struct NodeTestFixture
{
	NodeTestFixture(const fs::path& xml_path)
		: file(xml_path.string().c_str())
	{
		BOOST_REQUIRE(file.size());
		doc.parse<rapidxml::parse_full>(file.data());
	}


	void test_getnode_with_has(const std::string& path, bool has = true)
	{
		BOOST_CHECK_EQUAL(rxml::getnode(&doc, path) != nullptr, has);
	}


	rapidxml::xml_document<> doc;
	rapidxml::file<> file;
};




RXML_START_FIXTURE_TEST(NodeTestFixture, get_rxml_test_path() / "node-test-1.xml")

	RXML_FIXTURE_TEST(test_getnode_with_has, "node-test/xxx/sample");
	RXML_FIXTURE_TEST(test_getnode_with_has, "node-test/xxx/sample:attr");
	RXML_FIXTURE_TEST(test_getnode_with_has, "node-test/xxxx/sample", false);

RXML_END_FIXTURE_TEST()