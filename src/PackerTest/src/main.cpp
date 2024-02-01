

#include <catch2/catch_test_macros.hpp>
#include "Pack.h"
#include "TypeFactory.h"
#include "TypeBuilder.h"

typedef struct TEST_OPTIONS_
{
	int window_pos_x;
	int window_pos_y;
	int wiundow_size_x;
	int wiundow_size_y;
	std::string window_title;
	bool full_screen;
	bool vsync;
	bool show_fps;
} TEST_OPTIONS;

class TestOptionsBuilder : public TypeBuilder
{
public:
    TestOptionsBuilder(TypeFactory* pTypeFactory) : TypeBuilder(pTypeFactory) {};
    virtual ~TestOptionsBuilder() {};
    //BuildType is called to unpack an object
    virtual int BuildType(std::string key, any_type* object, Package pack)
    {
        //The following is the basic format that all TypeBuilders follow:
        int err = BUILD_OKAY;
        //Cast the object pointer to the type that this builder is responsible for building
        TEST_OPTIONS* ptr = (TEST_OPTIONS*)object;

        //Test that our key and type match the package we have.
        if (!pack->IsKey(key, GetBuilderTypeName()))
        {
            return BUILD_ERROR;
        }

        //For each member, use the TypeFactory to unpack the type(factories exist for all base types).
        err = m_TypeFactory->BuildTypeFromPackage("window_pos_x", &ptr->window_pos_x, pack);
        if (err != BUILD_OKAY) return BUILD_ERROR;

        err = m_TypeFactory->BuildTypeFromPackage("window_pos_y", &ptr->window_pos_y, pack);
		if (err != BUILD_OKAY) return BUILD_ERROR;

        err = m_TypeFactory->BuildTypeFromPackage("wiundow_size_x", &ptr->wiundow_size_x, pack);
		if (err != BUILD_OKAY) return BUILD_ERROR;

        err = m_TypeFactory->BuildTypeFromPackage("wiundow_size_y", &ptr->wiundow_size_y, pack);
		if (err != BUILD_OKAY) return BUILD_ERROR;

        err = m_TypeFactory->BuildTypeFromPackage("window_title", &ptr->window_title, pack);
		if (err != BUILD_OKAY) return BUILD_ERROR;

        err = m_TypeFactory->BuildTypeFromPackage("full_screen", &ptr->full_screen, pack);
		if (err != BUILD_OKAY) return BUILD_ERROR;

        err = m_TypeFactory->BuildTypeFromPackage("vsync", &ptr->vsync, pack);
		if (err != BUILD_OKAY) return BUILD_ERROR;

        err = m_TypeFactory->BuildTypeFromPackage("show_fps", &ptr->show_fps, pack);
		if (err != BUILD_OKAY) return BUILD_ERROR;

        return err;
    };
    //BuildPack is called to pack an object.
    virtual int BuildPack(std::string key, any_type* object, Package* pack)
    {
        int err = BUILD_OKAY;
        //Cast the object pointer to the correct type
        TEST_OPTIONS* ptr = (TEST_OPTIONS*)object;

        //MultiPacks can hold many values, ValuePacks only hold one value
        MultiPack* package = new MultiPack;
        package->SetKey(key); //set the key
        package->SetType(GetBuilderTypeName()); //set the type

        //add all members
        Package hold = NULL;
        err = m_TypeFactory->BuildPackageFromType("window_pos_x", &ptr->window_pos_x, &hold);
        if (err != BUILD_OKAY) return BUILD_ERROR;
        package->AddChild(hold);
        hold = NULL;

		err = m_TypeFactory->BuildPackageFromType("window_pos_y", &ptr->window_pos_y, &hold);
		if (err != BUILD_OKAY) return BUILD_ERROR;
		package->AddChild(hold);
		hold = NULL;

		err = m_TypeFactory->BuildPackageFromType("wiundow_size_x", &ptr->wiundow_size_x, &hold);
		if (err != BUILD_OKAY) return BUILD_ERROR;
		package->AddChild(hold);
		hold = NULL;

		err = m_TypeFactory->BuildPackageFromType("wiundow_size_y", &ptr->wiundow_size_y, &hold);
		if (err != BUILD_OKAY) return BUILD_ERROR;
		package->AddChild(hold);
		hold = NULL;

		err = m_TypeFactory->BuildPackageFromType("window_title", &ptr->window_title, &hold);
		if (err != BUILD_OKAY) return BUILD_ERROR;
		package->AddChild(hold);
		hold = NULL;

		err = m_TypeFactory->BuildPackageFromType("full_screen", &ptr->full_screen, &hold);
		if (err != BUILD_OKAY) return BUILD_ERROR;
		package->AddChild(hold);
		hold = NULL;

		err = m_TypeFactory->BuildPackageFromType("vsync", &ptr->vsync, &hold);
		if (err != BUILD_OKAY) return BUILD_ERROR;
		package->AddChild(hold);
		hold = NULL;

		err = m_TypeFactory->BuildPackageFromType("show_fps", &ptr->show_fps, &hold);
		if (err != BUILD_OKAY) return BUILD_ERROR;
		package->AddChild(hold);
		hold = NULL;

        (*pack) = package; //return the package

        return err;
    };
};


TEST_CASE("Test options", "[options]")
{
	TEST_OPTIONS options;
	options.window_pos_x = 100;
	options.window_pos_y = 100;
	options.wiundow_size_x = 800;
	options.wiundow_size_y = 600;
	options.window_title = "Test window";
	options.full_screen = false;
	options.vsync = true;
	options.show_fps = true;

	REQUIRE(options.window_pos_x == 100);
	REQUIRE(options.window_pos_y == 100);
	REQUIRE(options.wiundow_size_x == 800);
	REQUIRE(options.wiundow_size_y == 600);
	REQUIRE(options.window_title == "Test window");
	REQUIRE(options.full_screen == false);
	REQUIRE(options.vsync == true);
	REQUIRE(options.show_fps == true);
}

TEST_CASE("Test options can be packed", "[options]")
{
	TEST_OPTIONS options;
	options.window_pos_x = 200;
	options.window_pos_y = 200;
	options.wiundow_size_x = 1024;
	options.wiundow_size_y = 768;
	options.window_title = "Test window 2";
	options.full_screen = true;
	options.vsync = false;
	options.show_fps = false;


	//Register the default factories
	TypeFactory typeFactory;
	typeFactory.AddDefaultFactories(true, true);

	//register any custom factories
	//TypeFactory::RegisterNewFactory(NULL pointer to type that will be built, name of type, instance of the factory);
	typeFactory.RegisterNewFactory((TEST_OPTIONS*)0, "TEST_OPTIONS", new TestOptionsBuilder(&typeFactory));

	//Pack the object
	std::string pack;
	typeFactory.BuildStringFromType("TEST", &options, &pack);

	//Unpack the object
	TEST_OPTIONS testOut;
	//memset(&testOut, 0, sizeof(TEST_OPTIONS));
	typeFactory.BuildTypeFromString("TEST", &testOut, pack);

	REQUIRE(testOut.window_pos_x == 200);
	REQUIRE(testOut.window_pos_y == 200);
	REQUIRE(testOut.wiundow_size_x == 1024);
	REQUIRE(testOut.wiundow_size_y == 768);
	REQUIRE(testOut.window_title == "Test window 2");
	REQUIRE(testOut.full_screen == true);
	REQUIRE(testOut.vsync == false);
	REQUIRE(testOut.show_fps == false);
}