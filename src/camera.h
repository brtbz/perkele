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
	void IncreaseSizeBy(float multiplier); // zoom out
	void SetNewSize(vec2 new_size);
	void MoveToNewOrigo(vec2 new_origo);
};
void InitNewCamera();
