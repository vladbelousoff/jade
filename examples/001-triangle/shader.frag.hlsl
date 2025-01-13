struct PSOutput {
    float4 FragColor : SV_Target;
};

PSOutput main() {
    PSOutput output;
    output.FragColor = float4(1.0, 0.0, 0.0, 1.0); // Solid red color
    return output;
}
