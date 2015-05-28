turret = {}

function turret:create()
	local t = {}
	t.hp = 100;
	t.hit = hits:createCollide()
	print( t, "turret:create " )
	return t
end
function turret:destroy( t )
	print( t, "turret:destroy " )
	hits:destroyCollide( t.hit )
	t = nil;
end

function turret:think()
	print( self, "hp when think() = ", self.hp )
end

function turret:damage( hp )
	self.hp = self.hp - hp
	print( self, "hp after damage() = ", self.hp )
end


--[[
function createTurret()
	return turret:create()
end

function destroyTurret( t )
	return turret:destroy( t )
end

function thinkTurret( t )
end

function damageTurret( t )
end

hits:testFucntion()
obj = hits:createCollide()
obj:printInformation()
hits:destroyCollide( obj )
--]]

