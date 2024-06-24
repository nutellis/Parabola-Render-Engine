//#pragma once
//
//#include <Vector3.h>
//
//class Sphere
//{
//public:
//	Vector Center;
//
//	float radius, radius2;
//
//	Vector SurfaceColor, EmissionColor;
//	float Transparency, Reflection;
//
//	Sphere(const Vector &c,const float &r,const Vector &sc,const float &refl = 0,const float &transp = 0,const Vector &ec = 0) :
//		Center(c), radius(r), radius2(r * r), SurfaceColor(sc), EmissionColor(ec),Transparency(transp), Reflection(refl)
//	{}
//
//	bool intersect(const Vector &rayorig, const Vector &raydir, float &t0, float &t1) const
//	{
//		Vector l = Center - rayorig;
//		float tca = l.Dot(raydir);
//		if (tca < 0) return false;
//		float d2 = l.Dot(l) - tca * tca;
//		if (d2 > radius2) return false;
//		float thc = sqrt(radius2 - d2);
//		t0 = tca - thc;
//		t1 = tca + thc;
//
//		return true;
//	}
//};