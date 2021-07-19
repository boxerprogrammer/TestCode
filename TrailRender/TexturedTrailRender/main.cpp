#include<DxLib.h>
#include"Geometry.h"
#include<vector>
#include<list>
#include<cassert>
#include<array>
#include"ParticleSystem.h"

using namespace std;

Vector2 GetOrthogonalVector(const Vector2& v) {
	return { -v.y,v.x };
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	ChangeWindowMode(true);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	list<Position2> trail;

	constexpr size_t trail_limit = 10;
	ParticleSystem ps;
	int mouseInput = 0;
	int lastMouseInput = 0;
	while (ProcessMessage() != -1) {
		ClearDrawScreen();

		mouseInput = GetMouseInput();
		if (mouseInput & MOUSE_INPUT_LEFT && !(lastMouseInput & MOUSE_INPUT_LEFT)) {
			//パーティクル発生
			ps.Emit(32);
		}
		if (ps.IsTerminated()) {
			ps.ClearParticles();
		}
		int mx, my;
		GetMousePoint(&mx, &my);
		DrawCircleAA(mx, my, 15, 16, 0xffaaaa);
		Position2 lastpos(mx, my);
		float thickness = 30.0f;
		Vector2 lastRv = Vector2::Zero;
		for (auto& pos : trail) {
			//DrawLineAA(lastpos.x, lastpos.y, pos.x, pos.y, 0xffffff, thickness);
			Vector2 v = pos - lastpos;
			Vector2 rv = {};
			if (v == Vector2::Zero) {
				lastpos = pos;
				continue;
			}
			if (v != Vector2::Zero) {
				rv = GetOrthogonalVector(v).Normalized();// *(thickness * 0.5f);
				if (isnan(rv.x)) {
					lastpos = pos;
					continue;
				}
			}
			Vector2 rightV = rv;
			if (lastRv != Vector2::Zero) {
				if (isnan(lastRv.x)) {
					assert(0);
				}

				rightV = lastRv;// (rv + lastRv);
				if (rightV == Vector2::Zero) {
					lastpos = pos;
					continue;
				}
				rightV.Normalize();
			}
			assert(!isnan(rightV.x));

			Vector2 headV = rightV * (thickness * 0.5f);
			auto p1 = lastpos - headV;
			auto p2 = lastpos + headV;
			thickness *= 0.70f;
			headV=rv* (thickness * 0.5f);
			auto p0 = pos - headV;
			auto p3 = pos + headV;

			DrawQuadrangleAA(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, 0xffffff, true);
			lastpos = pos;
			lastRv = rv;
		}

		int ystr = 10;
		//for (auto& pos : trail) {
		//	DrawFormatString(10, ystr, 0xffffff, L"x=%02f,y=%02f", pos.x, pos.y);
		//	ystr += 20;
		//}

		trail.push_front(Position2(mx, my));
		if (trail.size() >= trail_limit) {
			trail.pop_back();
		}

		ps.Draw();

		auto drawcall = GetDrawCallCount();
		auto fps = GetFPS();

		DrawFormatString(10, 10, 0xfffffff, L"drawcall=%d", drawcall);
		DrawFormatString(10, 80, 0xfffffff, L"fps=%f", fps);

		ScreenFlip();
		lastMouseInput = mouseInput;
	}

	DxLib_End();
	return 0;
}