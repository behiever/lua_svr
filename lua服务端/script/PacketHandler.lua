--��Ӧmsgdef.h����Ϣid
C2S_LOGIN=0  --��½ 
S2C_LOGIN_Ret=1 --��½ret


--��Ϣ����
function ParsePacket( idx,data,len)
	print("ParsePacket")
 
	local b = buffer.new()
	b:init( data,len)
	cmd=b:read_int()

	print("cmd:" .. cmd )

	--��¼
	if cmd==C2S_LOGIN then
		On_C2S_LOGIN(idx,b)
		return
	end
	
	error("�޷�ʶ���cmd:",cmd)

	 
 
end


function On_C2S_LOGIN( idx,b)
 
		print("---On_C2S_LOGIN---")
		
	--int cmd; //����  C2S_LOGIN 
	--char name[20]; //�û���
	--char pwd[20];  //���� 

	--int cmd; //����  S2C_LOGIN_Ret
	--int errcode; //1=��½�ɹ�  , 0=��½ʧ��
	--char pwd[20];  //����

		--��ӡ ����
		local name=b:read_string(20) --�������ַ�����Ĵ�С
		local pwd=b:read_string(20) 
		print("name:" ..  name ,#name )
		print("pwd:" .. pwd ,#pwd )

		--�򵥵�½���
		if  name== "abc" and  pwd== "123456" then
			 
			print("��½�ɹ�"  )
			  
			--��½�ɹ�
			local ret = buffer.new()
			ret:write_int(S2C_LOGIN_Ret)  --cmd
			ret:write_int(1)  --errcode 
			ret:write_string ("pwdpwd",20) 
			--���͸��ͻ���
			G_NetSend(net,idx,ret:size(),ret:getptr())
			print("---On_C2S_LOGIN end---") 
			return
		end
		print("��½ʧ��"  )

		--��½ʧ��
		local ret = buffer.new()
		ret:write_int(S2C_LOGIN_Ret)   --cmd
		ret:write_int(0) --errcode 
		ret:write_string ("pwdpwd",20) 
		--���͸��ͻ���
		G_NetSend(net,idx,ret:size(),ret:getptr())

		print("---On_C2S_LOGIN end---") 

 
end

