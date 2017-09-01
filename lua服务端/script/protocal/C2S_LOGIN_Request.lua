


C2S_LOGIN_Request=Message:new{
	cmd,--
	name,--
	pwd,--	 
}

--C2S_LOGIN_Request=Message:new()

function C2S_LOGIN_Request:Init()
	self.cmd = 0
	self.name = 0
	self.pwd = 0
end

function C2S_LOGIN_Request:ReadFrom(data,len)
 
	local c = buffer.new()
	c:init( data,len)

	self.cmd=c:read_int()  
	self.name=c:read_string(20)
	self.pwd=c:read_string(20)

	return b
end


function C2S_LOGIN_Request:GetType()
	return RequestID("C2S_LOGIN")
end
