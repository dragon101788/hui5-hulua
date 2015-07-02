function split(str, split_char)
    local sub_str_tab = {};

    while (true) do
        local pos = string.find(str, split_char);
        if (not pos) then
            local size_t = #sub_str_tab
            table.insert(sub_str_tab,size_t+1,str);
            break;
        end

        local sub_str = string.sub(str, 1, pos - 1);
        local size_t = #sub_str_tab
        table.insert(sub_str_tab,size_t+1,sub_str);
        local t = string.len(str);
        str = string.sub(str, pos + 1, t);
    end
    return sub_str_tab;
end

function sleep(n)
   local t0 = os.clock()
   while os.clock() - t0 <= n do end
end

function proc_M109(mcu)

	while true
	do
		local ret = mcu:read();
		print("recv:",ret);
		if (ret == "ok") then
			return;
		end
		
		local t = {}
		for k, v in string.gmatch(ret, "(%w+):(%d+)") do
		    t[k]=v
		end
		for k, v in pairs(t) do
		    print(string.format("[%s][%s]",k, v))
		end
		wd_jct:command(string.format("%s",t["T"]));
	end
end


function proc_mcu_read(mcu)

	for ret in mcu:lines() do
		--print("recv:",ret);
		if (ret == "ok") then
			return;
		else
			print("return ",ret);
		end
	end
end


function mcu_output()
	wd_jct:command("set_text "..string.format("挤出头温度 %f",mcu.t));
	wd_rc:command("set_text "..string.format("热床温度 %f",mcu.e));
	fan_show:command("set_text "..string.format("风扇速度 %f",mcu.f));
	
	if mcu.stop == 1 then
		main_mov:command("stop");
	elseif mcu.stop == 0 then
		main_mov:command("start");
	end
	
	if mcu.fan_stop == 1 then
		fan_mov:command("stop");
	elseif mcu.fan_stop == 0 then
		fan_mov:command("start");
	end
end

co = coroutine.create(function (path)
		
	
    print("hello");
    file = io.open(path,"r");

    if file == nil then
        print("can't open file",path);
        return 5;
    end
		
		--os.execute("stty -F /dev/ttyS0 raw speed 115200");
		require("mcu");//libmcu.so
    mcu:open("/dev/ttyS0");
    i=0;
    for lbuf in file:lines() do
        i=i+1;

        n = string.find(lbuf,";");
        if (n == nil) or ( n>1 ) then
            local line = string.sub(lbuf,0,n);
            
            if(line ~= "") then
            	line = string.gsub(line, "^%s*;*(.-)%s*;*$", "%1")
	            key = split(line," ")[1];
	            value = string.sub(line,string.len(key)+1);
	            value = string.gsub(value, "^%s*;*(.-)%s*;*$", "%1")
	            print(string.format("[%s]:[%s]:[%s]",line,key,value));
							
							cmd:command("set_text " ..line);
							mcu:cmd(line);
							
            end
            
        end
        
        
    end

    file:close();
end)

function format_button_xml(x,y,icon,txt,cmd)
    return string.format(
    '	<button name="%s" x="%d" y="%d" width="120" height="120" lay="2" >\n'..
    '		<ldo><![CDATA[%s]]></ldo>'..
    '		<up>res/%s1.png</up>\n'..
    '		<dn>res/%s2.png</dn>\n'..
    '		<res meth="text" name="up" red="255" green="255" blue="255" dst_x="10" dst_y="80" font="song" size="18" txt="%s"/>\n'..
    '		<res meth="text" name="dn" red="255" green="255" blue="255" dst_x="10" dst_y="80" font="song" size="18" txt="%s"/>\n'..
    '	</button>\n'
    ,txt,x,y,cmd,icon,icon,txt,txt);
end


function ls_dir(dirpath)
    --os.execute("ls "' .. dirpath .. '" /s > temp.txt")
    local file = io.popen("ls ".. dirpath,"r" )
    local dirname = ""
    local filename = ""
    local xn = 0;
    local yn = 0;
    local xml = "<root>\n";
    xml = xml..format_button_xml(xn*120,yn*120,"sd","    主菜单","ui_main()");
    xn = xn + 1;
    
    for line in file:lines() do
       	if io.popen("if [ -d ".. line .." ] ; then echo 'yes' ;else echo 'no'; fi "):read() == "yes" then
            --lua_ls_dir(dirpath.."/"..line);
						xml = xml..format_button_xml(xn*120,yn*120,"dir",line,"ui_main()");
        else
            suffix = line:match(".+%.(%w+)$")
            if(suffix ~= nil)
            then
                --print(dirpath.."/"..line.."{"..suffix.."}")
                if(suffix =="gcode" or suffix == "GCODE" or suffix == "Gcode" or suffix == "gco" or suffix == "GCO")
                then
                    xml = xml..format_button_xml(xn*120,yn*120,"gco",line,string.format('print_gcoder("%s/%s")',dirpath,line));
                else
										xml = xml..format_button_xml(xn*120,yn*120,"file",line,"ui_main()");
                end
            else
                xml = xml..format_button_xml(xn*120,yn*120,"file",line,"ui_main()");
                
            end
        end
        print(string.format("%s xn=%d yn=%d",line,xn,yn));
        xn = xn + 1;
        if xn >= 4 then
        		xn = 0;
        		yn = yn + 1;
        end
     --print(line)
    end
    io.close(file);
    xml = xml .. "</root>\n";
    print(xml);
    frame:command("parsexml "..xml);
    ttt:command("hide on");
end

function lsdir1()

	print("lsdir");
end

function print_gcoder(path)
	print("print_gcoder ",path);
	jump("status.xml");
	import_xml(string.format('<root><luado>coroutine.resume(co,"%s")</luado></root>',path));
end	


function ui_print_sd()
	print("!!!!ui_print_sd!!!!");
	jump("pfordir.xml");
end

function ui_main()
	print("!!!!ui_print_sd!!!!");
	jump("main.xml");
end
