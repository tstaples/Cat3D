// TODO
// - default constructor (0, 0, 0, 1)
// - constructor taking axis and angle (v3, f32)
//      - q = (sin(theta/2) * v3) + cos(theta/2)
//      x = axis.x * sin(angle * 0.5f)
//      y = axis.y * sin(angle * 0.5f)
//      z = axis.z * sin(angle * 0.5f)
//      w = cos(angle * 0.5f)
// 
// Externals
// - create a helper to take a q and a v, which rotates the v by the q
//
// - Matrix Convert(quaternion)
//      - converts a q to a matrix
//
// Quaternion Slerp(Quaternion q1, q2, f32 t)