--对应msgdef.h的消息id
C2S_LOGIN=0  --登陆 
S2C_LOGIN_Ret=1 --登陆ret


--消息处理
function ParsePacket( idx,data,len)
	print("ParsePacket")
 
	local b = buffer.new()
	b:init( data,len)
	cmd=b:read_int()

	print("cmd:" .. cmd )

	--登录
	if cmd==C2S_LOGIN then
		On_C2S_LOGIN(idx,b)
		return
	end
	
	error("无法识别的cmd:",cmd)

	 
 
end


function On_C2S_LOGIN( idx,b)
 
		print("---On_C2S_LOGIN---")
		
	--int cmd; //命令  C2S_LOGIN 
	--char name[20]; //用户名
	--char pwd[20];  //密码 

	--int cmd; //命令  S2C_LOGIN_Ret
	--int errcode; //1=登陆成功  , 0=登陆失败
	--char pwd[20];  //密码

		--打印 数据
		local name=b:read_string(20) --参数是字符数组的大小
		local pwd=b:read_string(20) 
		print("name:" ..  name ,#name )
		print("pwd:" .. pwd ,#pwd )

		--简单登陆检查
		if  name== "abc" and  pwd== "123456" then
			 
			print("登陆成功"  )
			  
			--登陆成功
			local ret = buffer.new()
			ret:write_int(S2C_LOGIN_Ret)  --cmd
			ret:write_int(1)  --errcode 
			ret:write_string ("pwdpwd",20) 
			--发送给客户端
			G_NetSend(net,idx,ret:size(),ret:getptr())
			print("---On_C2S_LOGIN end---") 
			return
		end
		print("登陆失败"  )

		--登陆失败
		local ret = buffer.new()
		ret:write_int(S2C_LOGIN_Ret)   --cmd
		ret:write_int(0) --errcode 
		ret:write_string ("pwdpwd",20) 
		--发送给客户端
		G_NetSend(net,idx,ret:size(),ret:getptr())

		print("---On_C2S_LOGIN end---") 

 
end

