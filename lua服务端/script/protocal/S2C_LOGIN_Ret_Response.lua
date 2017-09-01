
 
-- 
S2C_LOGIN_Ret_Response =Message:new{
	cmd,--
	errcode,-- 
	pwd, 
 
}

function S2C_LOGIN_Ret_Response:Init()
	self.cmd = self.GetType()
	self.errcode = 0
	self.pwd = 0
end

function S2C_LOGIN_Ret_Response:WriteTo()
	self:dump()
	local ret = buffer.new()
	ret:write_int(self.GetType())
	ret:write_int(self.errcode)
	ret:write_bytes(self.pwd,20)
	return ret
end

function S2C_LOGIN_Ret_Response:GetType()
	return ResponseID("S2C_LOGIN_Ret")
end
