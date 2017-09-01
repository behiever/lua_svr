
RequestMap={}
RequestMap[0]="C2S_LOGIN"--登陆 
 


ResponseMap={}
ResponseMap[1]="S2C_LOGIN_Ret"--登陆 
 


--协议名查找对应下标  用C2S_LOGIN 可以查找到0
function RequestID(name)
	for k, v in pairs(RequestMap) do
		if v==name then
			return k
		end
	    --print(k, v)

	end
	error("RequestID name["..name .."] not found!!!")
	return nil
end

function ResponseID(name)
	for k, v in pairs(ResponseMap) do
		if v==name then
			return k
		end
	    --print(k, v)

	end
	error("ResponseID name["..name.."] not found!!!")
	return nil
end