#pragma once
#include"Vector3.h"
#include"SmartPointer.h"
namespace GStar {
	class Ray;
	class material;
	struct hit_record {
		float t;
		Vector3 p;
		Vector3 n;
		ObserverPointer<material> mat_ptr;
	};
	class Hitable {
	public:
		static constexpr float DEFAULT_RAY_MIN = 0.001f;
		static constexpr float DEFAULT_RAY_MAX = 1000.0f;
		static hit_record record;
		virtual bool hit(const Ray& ray, float t_min = DEFAULT_RAY_MIN, float t_max = DEFAULT_RAY_MAX, hit_record& rec = record) const = 0;
		virtual ~Hitable() {}
	};
}