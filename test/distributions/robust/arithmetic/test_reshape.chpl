use driver;

const D1: domain(1, int) distributed Dist1D = [1..24];
const D2: domain(2, int) distributed Dist2D = [1..4, 1..6];
const D3: domain(3, int) distributed Dist3D = [1..2, 1..4, 1..3];

var A1: [i in D1] int = i;

writeln(A1);

var A2 = reshape(A1, D2);

writeln(A2);

var A3 = reshape(A2, D3);

writeln(A3);

var A4 = reshape(A3, [1..24]);

writeln(A4);

writeln(A1.domain.dist == D1.dist);
writeln(A2.domain.dist == D2.dist);
writeln(A3.domain.dist == D3.dist);
writeln(A4.domain.dist != D1.dist);
