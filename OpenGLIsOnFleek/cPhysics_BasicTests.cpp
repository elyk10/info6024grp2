#include "cPhysics.h"

//== = Section 5.2.7: ============================================================ =
//
// Returns true if sphere s intersects triangle ABC, false otherwise.
bool cPhysics::m_TestSphereTriangle(float sphereRadius, glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2,
						glm::vec3 sphereCentre)
{
	// Find point P on triangle ABC closest to sphere center
	glm::vec3 closestPoint = this->m_ClosestPtPointTriangle(sphereCentre, vert0, vert1, vert2);

	// Sphere and triangle intersect if the (squared) distance from sphere
	// center to point p is less than the (squared) sphere radius
	glm::vec3 v = closestPoint - sphereCentre;

	bool isItIntersecting = false;

	if (glm::dot(v, v) <= (sphereRadius * sphereRadius))
	{
		isItIntersecting = true;
	}

	return isItIntersecting;
}


// From: Real-Time Collision Detection- Ericson, Christer- 9781558607323- Books - Amazon.ca
// https://www.amazon.ca/Real-Time-Collision-Detection-Christer-Ericson/dp/1558607323/ref=pd_lpo_sccl_1/137-6663593-0701065?pd_rd_w=YiI8A&content-id=amzn1.sym.687e7c56-2b08-4044-894f-bbad969cf967&pf_rd_p=687e7c56-2b08-4044-894f-bbad969cf967&pf_rd_r=JWS56NJC99QEH56TYFJX&pd_rd_wg=zBE6V&pd_rd_r=d611733e-ec29-4b30-bd70-89f092f1991a&pd_rd_i=1558607323&psc=1
// Chapter 5:
glm::vec3 cPhysics::m_ClosestPtPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 bc = c - b;

	// Compute parametric position s for projection P' of P on AB,
	// P' = A + s*AB, s = snom/(snom+sdenom)
	float snom = glm::dot(p - a, ab), sdenom = glm::dot(p - b, a - b);

	// Compute parametric position t for projection P' of P on AC,
	// P' = A + t*AC, s = tnom/(tnom+tdenom)
	float tnom = glm::dot(p - a, ac), tdenom = glm::dot(p - c, a - c);

	if (snom <= 0.0f && tnom <= 0.0f) return a; // Vertex region early out

	// Compute parametric position u for projection P' of P on BC,
	// P' = B + u*BC, u = unom/(unom+udenom)
	float unom = glm::dot(p - b, bc), udenom = glm::dot(p - c, b - c);

	if (sdenom <= 0.0f && unom <= 0.0f) return b; // Vertex region early out
	if (tdenom <= 0.0f && udenom <= 0.0f) return c; // Vertex region early out


	// P is outside (or on) AB if the triple scalar product [N PA PB] <= 0
	glm::vec3 n = glm::cross(b - a, c - a);
	float vc = glm::dot(n, glm::cross(a - p, b - p));
	// If P outside AB and within feature region of AB,
	// return projection of P onto AB
	if (vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
		return a + snom / (snom + sdenom) * ab;

	// P is outside (or on) BC if the triple scalar product [N PB PC] <= 0
	float va = glm::dot(n, glm::cross(b - p, c - p));
	// If P outside BC and within feature region of BC,
	// return projection of P onto BC
	if (va <= 0.0f && unom >= 0.0f && udenom >= 0.0f)
		return b + unom / (unom + udenom) * bc;

	// P is outside (or on) CA if the triple scalar product [N PC PA] <= 0
	float vb = glm::dot(n, glm::cross(c - p, a - p));
	// If P outside CA and within feature region of CA,
	// return projection of P onto CA
	if (vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
		return a + tnom / (tnom + tdenom) * ac;

	// P must project inside face region. Compute Q using barycentric coordinates
	float u = va / (va + vb + vc);
	float v = vb / (va + vb + vc);
	float w = 1.0f - u - v; // = vc / (va + vb + vc)
	return u * a + v * b + w * c;
}

// The OG code from the book:
//Point ClosestPtPointTriangle(Point p, Point a, Point b, Point c)
//{
//	Vector ab = b - a;
//	Vector ac = c - a;
//	Vector bc = c - b;
//
//	// Compute parametric position s for projection P' of P on AB,
//	// P' = A + s*AB, s = snom/(snom+sdenom)
//	float snom = Dot(p - a, ab), sdenom = Dot(p - b, a - b);
//
//	// Compute parametric position t for projection P' of P on AC,
//	// P' = A + t*AC, s = tnom/(tnom+tdenom)
//	float tnom = Dot(p - a, ac), tdenom = Dot(p - c, a - c);
//
//	if (snom <= 0.0f && tnom <= 0.0f) return a; // Vertex region early out
//
//	// Compute parametric position u for projection P' of P on BC,
//	// P' = B + u*BC, u = unom/(unom+udenom)
//	float unom = Dot(p - b, bc), udenom = Dot(p - c, b - c);
//
//	if (sdenom <= 0.0f && unom <= 0.0f) return b; // Vertex region early out
//	if (tdenom <= 0.0f && udenom <= 0.0f) return c; // Vertex region early out
//
//
//	// P is outside (or on) AB if the triple scalar product [N PA PB] <= 0
//	Vector n = Cross(b - a, c - a);
//	float vc = Dot(n, Cross(a - p, b - p));
//	// If P outside AB and within feature region of AB,
//	// return projection of P onto AB
//	if (vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
//		return a + snom / (snom + sdenom) * ab;
//
//	// P is outside (or on) BC if the triple scalar product [N PB PC] <= 0
//	float va = Dot(n, Cross(b - p, c - p));
//	// If P outside BC and within feature region of BC,
//	// return projection of P onto BC
//	if (va <= 0.0f && unom >= 0.0f && udenom >= 0.0f)
//		return b + unom / (unom + udenom) * bc;
//
//	// P is outside (or on) CA if the triple scalar product [N PC PA] <= 0
//	float vb = Dot(n, Cross(c - p, a - p));
//	// If P outside CA and within feature region of CA,
//	// return projection of P onto CA
//	if (vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
//		return a + tnom / (tnom + tdenom) * ac;
//
//	// P must project inside face region. Compute Q using barycentric coordinates
//	float u = va / (va + vb + vc);
//	float v = vb / (va + vb + vc);
//	float w = 1.0f - u - v; // = vc / (va + vb + vc)
//	return u * a + v * b + w * c;
//}

