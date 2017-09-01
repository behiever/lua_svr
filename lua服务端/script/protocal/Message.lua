
Message={
}

function Message:new(o)
	o=o or {}
	setmetatable(o,self)
	self.__index=self
	return o
end

function Message:GetArg(name)
	return self.name
end
 
function Message:WriteTo()
 
end

function Message:dump()
	print("------Message:dump--------------")
	for k, v in pairs(self) do

	    print(k, v)

	end
	print("------Message:dump end--------------")
end

 

function Message:Send(idx)
	local response=self:WriteTo()
	local len=response:size()
	G_NetSend(net,idx,  len,response:getptr() )
	print("G_NetSend idx["..idx.."] len:"..len )
end