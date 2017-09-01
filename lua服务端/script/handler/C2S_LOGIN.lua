
dofile("script/protocal/S2C_LOGIN_Ret_Response.lua")

function On_C2S_LOGIN( idx,request)
 
		print("---On_C2S_LOGIN---")
		
		--打印request成员数据
		request:dump()

		--简单登陆检查
		if request.name== "abc" and request.pwd== "123456" then
			 
			print("11111111"  )
			  
			--登陆成功
			local ret=S2C_LOGIN_Ret_Response:new()
			ret:Init()
			ret.errcode=1
			ret.pwd ="pwdpwd" 
			ret:Send(idx)
			print("---On_C2S_LOGIN end---") 
			return
		end
		print("22222222"  )

		--登陆失败
		local ret=S2C_LOGIN_Ret_Response:new()
		ret:Init()
		ret.errcode=0
		ret.pwd ="pwdpwd" 
		ret:Send(idx)

		print("---On_C2S_LOGIN end---")
	--local player = world:GetPlayer(user)

	--local resultID = player.actionFSM:AwardTask(request.taskID)
	--if resultID then return ErrorReturn(user, resultID) end

	--dofile("lua/core/protocal/AwardTaskEvent.lua")
	--local event=AwardTaskEvent:new()
	--event:Init()
	--event.taskID=request.taskID

	--local i=1
	--event.taskList = {}
	--for task_id, task_state in pairs(player.tasks) do

	--	event.taskList[i]={}
	--	event.taskList[i].taskID = task_id
	--	event.taskList[i].taskState = task_state

	--	i=i+1
	--end

	--event:Send(user)
end

