#include <iostream>
#include "hulua.h"

class kJCollideObject
{
public:
	kJCollideObject( void )
	{
		std::cout << this << ' ' << __FUNCTION__ << std::endl;
	}
	~kJCollideObject( void )
	{
		std::cout << this << ' ' << __FUNCTION__ << std::endl;
	}

	void printInformation( void )
	{
		std::cout << this << ' ' << __FUNCTION__ << std::endl;
	}
};
class kJCollideCreator
{
public:
	kJCollideObject *createCollide( void )
	{
		return new kJCollideObject();
	}
	void destroyCollide( kJCollideObject *obj )
	{
		delete obj;
	}

	void testFucntion( void )
	{
		std::cout << __FUNCTION__ << std::endl;
	}
};

int main( int argc, char *argv[] )
{
	kJCollideCreator	hits;

	hulua::lua_context L;

	hulua::class_add< kJCollideObject >( L, "kJCollideObject" );
	hulua::class_def< kJCollideObject >( L, "printInformation", &kJCollideObject::printInformation );

	hulua::class_add< kJCollideCreator >( L, "kJCollideCreator" );
	hulua::class_def< kJCollideCreator >( L, "destroyCollide", &kJCollideCreator::destroyCollide );
	hulua::class_def< kJCollideCreator >( L, "createCollide", &kJCollideCreator::createCollide );
	hulua::class_def< kJCollideCreator >( L, "testFucntion", &kJCollideCreator::testFucntion );
	hulua::set( L, "hits", &hits );

	hulua::dofile( L, "sample6.lua" );

	hulua::table turret = hulua::get< hulua::table >( L, "turret" );
	hulua::table obj1 = turret.call< hulua::table >( "create", turret );
	turret.call< void >( "think", obj1 );
	turret.call< void >( "damage", obj1, 10 );
	turret.call< void >( "destroy", turret, obj1 );

//	lua_tinker::table obj2 = lua_tinker::call< lua_tinker::table >( L, "createTurret" );
//	lua_tinker::call< void >( L, "thinkTurret", obj2 );
//	lua_tinker::call< void >( L, "damageTurret", obj2 );
//	lua_tinker::call< void >( L, "destroyTurret", obj2 );

	return 0;
}

