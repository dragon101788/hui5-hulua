


coa = coroutine.create(function (path)
	while true do
	print("a");
	end
end)

cob = coroutine.create(function (path)

	while true do
	print("b");
	end
end)


coroutine.resume(coa,"test.gcode")
coroutine.resume(cob,"test.gcode")
