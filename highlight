import subprocess  
val = '50cad30a2a074557e1937708bbd83923b5d35b3bbf5fd13f99270f4bb78823159cfe3e840b698f0595c7b52f8837862a1c96ec39554345c84a8d690b9f118b20'  
vallst=[val[0:32],val[32:64],val[64:96],val[96:]]  
def solve(vallst):  
    res=[]  
    for t in range(3):  
        num=''  
        lst=[]  
        for i in range(16):  
            tmpiv = 0  
            strtmpiv=''  
            for j in range(16**2):  
                strtmpiv=(32-len(lst)*2-len(hex(tmpiv)[2:]))*'0'+hex(tmpiv)[2:]  
                if len(lst)!=0:  
                    for k in range(len(lst)):  
                        if len(hex(int(lst[k],16)^(i+1))[2:])==1:  
                            tmp1='0'+hex(int(lst[k],16)^(i+1))[2:]  
                        else:  
                            tmp1=hex(int(lst[k],16)^(i+1))[2:]  
                        strtmpiv+=tmp1  
                output=subprocess.call(['dec_oracle', strtmpiv+vallst[3-t]])  
                if output==200:  
                    if len(hex(tmpiv^(i+1))[2:])==1:  
                        lst.insert(0,'0'+hex(tmpiv^(i+1))[2:])  
                    else:  
                        lst.insert(0,hex(tmpiv^(i+1))[2:])  
                    break  
                tmpiv=tmpiv+1  
        for item in lst:  
            num+=item  
        tmp=hex(int(num,16)^int(vallst[2-t],16))[2:]  
        if len(tmp)<32:  
            res.insert(0,(32-len(tmp))*'0'+tmp)  
        else:  
            res.insert(0,tmp)  
    return res  
res=solve(vallst)  
print(res)  
