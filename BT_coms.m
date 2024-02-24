clear all,
device = bluetooth('ESP32 Gostosa');
ready=0;
data=[];
result=[];
while (1)
    data=[];
    while(ready==0)
        data(1,1) = read(device, 1);
        if (data(1)==10)
            while(ready~=1)
                data(1,end+1)=read(device, 1);
                if(data(end)==13)
                    ready=1;
                end
            end
        end
    end
    ready=0;
        
    p10=1;
    p13=1;
    pos10=find(data==10);
    pos13=find(data==13);
    mult=[1,10,100,1000];
    
    
    start=pos10(p10)+1;
    finish=pos13(p13)-1;
    
    i=start:finish;
    result(1,end+1)=sum(flip( data(i)-48).*mult(1:length(i)));
    plot(1:length(result),result)
    p10=p10+1;
    p13=p13+1;
       
     
end