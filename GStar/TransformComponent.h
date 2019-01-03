#pragma once
#include "Coordinate.h"
#include "Component.h"
#include "View.h"
#include "Object.h"
#include "MyString.h"
namespace GStar {
	//Version 2.0 Check OLDCODE Folder for previous version
	//Support default transform based on world, parent and self coordinate
	struct TransformData
	{
		GStar::Matrix4 M;
		GStar::Matrix4 MI;
		GStar::Matrix4 CR;
		GStar::Vector3 Ca;
		GStar::Vector3 So;
		GStar::Vector3 Sn;
		bool RotationUpdate;
		bool TransformUpdate;
		bool ScaleUpdate;
	};
	enum Base { WORLD = 0, PARENT = 1, SELF=2 };
	enum Layer { DEFAULT = 0, CAMERA = 1, LIGHT = 2 };
	class TransformComponent :public Component {
	public:
		inline GStar::Matrix4 getModel() const { return my_model.M; }
		inline GStar::Matrix4 getInverseModel() const { 
			float a = 0.0f;
			return my_model.MI; 
		
		}
		inline GStar::Matrix4 GetBaseMatrix() const {
			//R = M * So^-1
			GStar::Matrix4 temp = my_model.M;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					temp.Getreference(j, i) /= my_model.So.getValue(i);
				}
			}
			LeftApplyingTransform(temp, Vector3(0, 0, 0));
			return temp;
		}
		inline GStar::Matrix4 GetBaseMatrixInverse() const {
			//R^(-1) = So*M^-1, R^(-1) = Rt
			GStar::Matrix4 temp = my_model.MI;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					temp.Getreference(i, j) *= my_model.So.getValue(i);
				}
			}
			LeftApplyingTransform(temp, Vector3(0, 0, 0));
			return temp;
		}
		inline GStar::Vector3 GetTransform() const {
			return Vector3(my_model.M.GetData(0, 3), my_model.M.GetData(1, 3), my_model.M.GetData(1, 3));
		}
		inline void SetTransform(const GStar::Vector3& transform, Base base) {
			if (base == Base::PARENT) {
				my_model.Ca = transform;
			}
			else if (base == Base::SELF) {
				my_model.Ca = GetBaseMatrix()*transform;
			}
			else
			{
				my_model.Ca = transform;
			}
			if (my_parent) {
				my_model.Ca = my_parent->GetBaseMatrixInverse()*my_model.Ca;
			}
			my_model.TransformUpdate = true;
		}
		inline void Translate(const GStar::Vector3& translate, Base base) {
			Vector3 delta;
			if (base == Base::PARENT) {
				delta = translate;
			}
			else if (base == Base::SELF) {
				GStar::Matrix4 matrix = GetBaseMatrix();
				delta = matrix*translate;
			}
			else {
				delta = translate;
			}
			if (my_parent) {
				delta = my_parent->GetBaseMatrixInverse()*my_model.Ca;
			}
			my_model.Ca += delta;
			my_model.TransformUpdate = true;
		}
		inline void Rotate(const GStar::Vector3& rotation) {
			Rotate(rotation.x(), rotation.y(), rotation.z());
			return;
		}
		inline void Rotate(float x, float y, float z) {
			array_ff temp = IDENTICAL_MATRIX;
			float sx = sin(x*PI / 180);
			float sy = sin(y*PI / 180);
			float sz = sin(z*PI / 180);
			float cx = cos(x*PI / 180);
			float cy = cos(y*PI / 180);
			float cz = cos(z*PI / 180);
			//Rotate z x y
			temp[0][0] = cz * cy - sz * sx*sy;
			temp[0][1] = -sz * cx;
			temp[0][2] = sy * cz + sz * sx*cy;
			temp[1][0] = sz * cy + cz * sx*sy;
			temp[1][1] = cz * cx;
			temp[1][2] = sz * sy - cz * sx*cy;
			temp[2][0] = -cx * sy;
			temp[2][1] = sx;
			temp[2][2] = cx * cy;
			//Why should I mulitply it in this way?
			//Why is it correct?
			my_model.CR = my_model.CR.Dot(GStar::Matrix4(temp));
			my_model.RotationUpdate = true;
			return;
		}inline void SetScale(float x, float y, float z) {
			my_model.Sn[0] = x;
			my_model.Sn[1] = y;
			my_model.Sn[2] = z;
			my_model.ScaleUpdate = true;
			return;
		} inline void SetScale(const GStar::Vector3& scale) {
			my_model.Sn = scale;
			my_model.ScaleUpdate = true;
			return;
		}
		inline bool TransformUpdate() const{ return my_model.TransformUpdate; }
		inline bool RotationUpdate() const{ return  my_model.RotationUpdate; }
		inline bool ScaleUpdate() const{ return my_model.ScaleUpdate; }
		inline void SetUpdate(bool trans, bool rotate, bool scale) {
			my_model.TransformUpdate |= trans;
			my_model.RotationUpdate |= rotate;
			my_model.ScaleUpdate |= scale;
			return;
		}
		inline void UpdateChildrenFlags(){
			bool RotateUpdate = false;
			bool TransformUpdate = false;
			if (my_model.RotationUpdate) {
				RotateUpdate = true;
				TransformUpdate = true;
			}
			else if (my_model.TransformUpdate) {
				TransformUpdate = true;
			}
			my_children.Resetcurrent();
			while (my_children.HasNext()) {
				my_children.GetNext()->SetUpdate(RotateUpdate, TransformUpdate, false);
				my_children.Move();
			}
		}
		inline void Update(float deltatime) {
			if (my_model.RotationUpdate || my_model.TransformUpdate || my_model.ScaleUpdate) {
				UpdateChildrenFlags();
				GStar::Matrix4 temp;//R2
				if (my_model.RotationUpdate) {
					temp = my_model.CR;// deltaR * R2
					if (my_parent) {
						temp = temp.Dot(my_parent->GetBaseMatrix());// deltar* R2 * R1
					}
					my_model.RotationUpdate = false;
				}
				else {
					temp = GetBaseMatrix();
				}
				GStar::Vector3 offset = GetTransform();
				if (my_model.TransformUpdate) {
					if (my_parent) {
						offset = my_parent->GetBaseMatrix()* my_model.Ca;
					}
					else {
						offset = my_model.Ca;
					}
					my_model.TransformUpdate = false;
				}
				if (my_model.ScaleUpdate) {
					// Here set So to Sn invoke GetBaseMatix or Inverse will lead to
					//mistake before set up the new M and MI
					my_model.So = my_model.Sn;

					my_model.ScaleUpdate = false;
				}
				GStar::Vector3 Scale = my_model.So;
				my_model.M = temp;
				my_model.MI = temp.T();
				RightApplyingScale(my_model.M, Scale);
				LeftApplyingScale(my_model.MI, 1.0f / Scale);
				LeftApplyingTransform(my_model.M, offset);
				GStar::Vector3 InverseOffset = -1.0f*(my_model.MI*offset);
				LeftApplyingTransform(my_model.MI, InverseOffset);
				float a = 0;
			}
		}
		/*same with generating a 3*3 diagnoal matrix by the vector and use the up-left 3*3 part of the matrix to multiply it.
		does not concern other parts of the matrix*/
		inline void RightApplyingScale(GStar::Matrix4& matrix, const GStar::Vector3& Scale) const{
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					matrix.Getreference(j, i) *= Scale.getValue(i);
				}
			}
		}
		/*same with generating a 3*3 diagnoal matrix by the vector,multiply it with the up-left 3*3 part of the matrix.
		does not concern other parts of the matrix*/
		inline void LeftApplyingScale(GStar::Matrix4& matrix, const GStar::Vector3& Scale) const{
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					matrix.Getreference(i, j) *= Scale.getValue(i);
				}
			}
		}
		inline void LeftApplyingTransform(GStar::Matrix4& matrix, const GStar::Vector3& Scale) const{
			for (int j = 0; j < 3; j++) {
				matrix.Getreference(j, 3) = Scale.getValue(j);
			}
		}
		TransformComponent(Object* object, const GStar::MyString& name) :
			my_model{ GStar::Matrix4(IDENTICAL_MATRIX),//M
			GStar::Matrix4(IDENTICAL_MATRIX),//MI
			GStar::Matrix4(IDENTICAL_MATRIX),//DeltaR
			GStar::Vector3(0,0,0),//Ca
			GStar::Vector3(1,1,1),//So
			GStar::Vector3(1,1,1),//Sn
			false,
			false,
			false,
			},
			Component(TRANSFORM_WORD),
			my_parent(nullptr),
			my_Object(object),
			my_layer(Layer::DEFAULT),
			my_name(GStar::MyString::hash_str(name.GetString())){}
		inline void WorldUpdate(float deltatime) {
			my_Object->Update(deltatime);
			my_children.Resetcurrent();
			while (my_children.HasNext()) {
				my_children.GetNext()->WorldUpdate(deltatime);
				my_children.Move();
			} 
		}
		inline void AddChildren(TransformComponent* component) {
			my_children.Push(component);
			component->SetParent(this);
		}
		inline void SetParent(TransformComponent* component) {
			my_parent = component;
		}inline Layer getLayer() const {
			return my_layer;
		}inline void SetLayer(GStar::Layer layer) {
			my_layer = layer;
		}
		inline unsigned int GetName() const {
			return my_name;
		}inline void SetName(const GStar::MyString& name) {
			my_name = GStar::MyString::hash_str(name.GetString());
		}
	private:
		TransformData my_model;
		SingleLinkedList<GStar::TransformComponent*> my_children;
		GStar::TransformComponent* my_parent;
		Object* my_Object;
		Layer my_layer;
		unsigned int my_name;
	};
}