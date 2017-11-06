t = 2999
[x] = textread('LOSS.txt','%f');
for i=1:50
scatter(i,x(i),'.m');
line([i,i+1],[x(i),x(i+1)]);
hold on;
end