struct Camera
{
private:
	vec2 min;
	vec2 max;
	vec2 origo;
	vec2 size;
public:
	vec2 Min(){ return min; }
	vec2 Max(){ return max; }
	vec2 Origo() { return origo; }
	vec2 Size() { return size; }

	void SetCamera(vec2 _min, vec2 _max);
	void MoveToNewMin(vec2 new_min);
	void IncreaseSizeBy(float multiplier, vec2 mouse_pos); // zoom out
	void ConstrainSize();
	vec2 ConstrainPositionOrigo();
	vec2 ConstrainThisOrigo(vec2 origo_seeking_approval);
	void SetNewSize(vec2 new_size);
	void MoveToNewOrigo(vec2 new_origo);
};
void InitNewCamera();

// camera is a mess!

/*
typedef struct CameraYeah
{
	float aspect_ratio;
	vec2 origo;
	vec2 viewport_size;
	float max_zoom;
	float min_zoom;
} CameraYeah;

void CameraPositionOffset(CameraYeah *c, vec2 offset);
void CameraPositionNew(CameraYeah *c, vec2 new_position);
void CameraPositionCoverTheseCoordinates(CameraYeah *c, vec2 first_position, vec2 second_position); // these are not necessarily corners, but they might (depends on camera aspect ratio)
*/