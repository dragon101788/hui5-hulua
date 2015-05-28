
print("lua script func.lua have been load ")

function showinfo()
print("welcome to  lua world ")
end

function showstr(str)
print("The string you input is " .. str)
end


function add(x,y)
	return x+y;
end

function laverage()

dragon_lua(5,6,7);
avg,sum = average(10,20,30,40,50);
print("The average is ",avg);
print("The sum is ",sum);

end

function test_struct(a)
return string.format("结构体测试 : a.a = %d a.c.b=%d a.str=%s",a.a, a.c.b, a.str);
end

ele =
{
	path="abc",
	x=10,
	y=20,
	10,
	node={7,8,9,45,32,6},
	attribute={a=7,b=8,c=9,d=45,e=32,f=6},
};
