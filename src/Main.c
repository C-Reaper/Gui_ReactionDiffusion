#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/ReactionDiffusion.h"

ReactionDiffusion rd;
TransformedView tv;

void Setup(AlxWindow* w){
	tv = TransformedView_Make(
		(Vec2){ GetWidth(),GetHeight() },
		(Vec2){ 0.0f,0.0f },
		(Vec2){ 0.01f,0.01f },
		(float)GetWidth() / (float)GetHeight()
	);
    AlxFont_Resize(&window.font,50,50);

    rd = ReactionDiffusion_New(500U,500U);
}
void Update(AlxWindow* w){
    tv.ZoomSpeed = (float)w->ElapsedTime;
	TransformedView_HandlePanZoom(&tv,window.Strokes,(Vec2){ GetMouse().x,GetMouse().y });
	TransformedView_Output(&tv,(Vec2){ GetWidth(),GetHeight() });

	if(Stroke(ALX_MOUSE_L).DOWN){
		const Vec2 wm = TransformedView_ScreenWorldPos(&tv,GetMouse());

		ReactionDiffusion_Place(
			&rd,
			10U,
			(unsigned int)wm.x,
			(unsigned int)wm.y,
			(ReactionDiffusion_Chemical){
				.a = 1.0,
				.b = 1.0
			}
		);
	}
	//if(Stroke(ALX_KEY_SPACE).DOWN){
		ReactionDiffusion_Iter(&rd,40.0 * w->ElapsedTime);
	//}
	if(Stroke(ALX_KEY_R).PRESSED){
		ReactionDiffusion_Reset(&rd);
	}

	Clear(BLACK);

    const Vec2 tl = TransformedView_ScreenWorldPos(&tv,(Vec2){ 0.0f,0.0f });
	const Vec2 br = TransformedView_ScreenWorldPos(&tv,(Vec2){ GetWidth(),GetHeight() });
	
	for(int i = (int)F32_Floor(tl.y) - 1;i<(int)F32_Ceil(br.y);i++){
		for(int j = (int)F32_Floor(tl.x) - 1;j<(int)F32_Ceil(br.x);j++){
			if(j<0 || j>=rd.width || i<0 || i>=rd.height) continue;
            
            const int ind = i * rd.width + j;
			const Vec2 bg_p = TransformedView_WorldScreenPos(&tv,(Vec2){ j,i });
			const Vec2 bg_d = TransformedView_WorldScreenLength(&tv,(Vec2){ 1.0f,1.0f });
			const float l = F32_Clamp((float)(rd.data[ind].a - rd.data[ind].b),0.0f,1.0f);
			RenderRect(bg_p.x,bg_p.y,bg_d.x + 1,bg_d.y + 1,Pixel_toRGBA(l,l,l,1.0f));
		}
	}
}
void Delete(AlxWindow* w){
    ReactionDiffusion_Free(&rd);
}

int main(){
    if(Create("Reaction Diffusion",1800,1000,1,1,Setup,Update,Delete))
        Start();
    return 0;
}