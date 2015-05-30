

function main()
	print("lua main\n");
	register_class("abc");
	register_class("aaa");
	abc.x = 20;
	abc.y = 30;
	aaa.x = 199;
	aaa.y = 299;
	abc:showinfo();
	aaa:showinfo();
end
