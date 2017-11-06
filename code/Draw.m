n = 685;
[x,y] = textread('Vertex.txt','%f%f');
m = 1282;
[u,v] = textread('bus685.txt','%d%d');

u = int32(u);
v = int32(v);

for i=1:n
scatter(x(i),y(i),'.m');
hold on;
 end
for i=1:m
line([x(u(i) + 1),x(v(i) + 1)],[y(u(i) + 1),y(v(i) + 1)]);
hold on;
end