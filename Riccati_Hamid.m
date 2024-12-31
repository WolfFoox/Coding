A=[0 1;0 0];B=[0;1];
Q=[1 0;0 ];
R=[1];
H=[A -B*inv(R)*B';-Q -A'];
rank(H)
[X,D]=eig(H)
j=1;
for i=1:rank(H)
    if real(D(i,i))<0
        temp(:,i)=X(:,j);
        j=j+1;
    end
end
j=j-1;
for i=1:j
    v1(i,:)=temp(i,:);
    t=i+j;
    v2(i,:)=temp(t,:);
end
p=v2*(inv(v1))