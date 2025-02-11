#include "scene.h"
#include <cassert>
#include <cstring> // for strcmp
#include <sstream>
#include <stdexcept>

// additional libraries
#include "happly/happly.h"
#include <chrono>
#include <thread>
Scene::Scene()
{ 
    p_bounding_volume = nullptr;
    background_color = Vec3i();
    shadow_ray_epsilon = 0;
    test_epsilon = 0;
    max_recursion_depth = 0;
    ambient_light = Vector3();
    bg_texture = nullptr;
    spherical_dir_light = nullptr;
    random_device rd; 
    gRandomGenerator = mt19937(rd());
}

/**
 * Parses the xml file and loads the information in data structure.
 *
 * @param filepath contains the xml file path
 */
void Scene::loadFromXml(const string &filepath)
{
    tinyxml2::XMLDocument file;
    stringstream stream;
    if (XML_PARSE_LOGS)
        cout << "filepath = " << filepath << endl;
    auto res = file.LoadFile(filepath.c_str());
    if (res)
    {
        throw runtime_error("Error: The xml file cannot be loaded.");
    }

    auto root = file.FirstChild();
    if (!root)
    {
        throw runtime_error("Error: Root is not found.");
    }

    // Get BackgroundColor
    auto element = root->FirstChildElement("BackgroundColor");
    if (element)
    {
        stream << element->GetText() << endl;
    }
    else
    {
        stream << "0 0 0" << endl;
    }
    stream >> background_color.x >> background_color.y >> background_color.z;
    if (XML_PARSE_LOGS)
        cout << "background_color = " << background_color.x << " " << background_color.y << " " << background_color.z
             << endl;

    // Get ShadowRayEpsilon
    element = root->FirstChildElement("ShadowRayEpsilon");
    if (element)
    {
        stream << element->GetText() << endl;
    }
    else
    {
        stream << "0.001" << endl;
    }
    stream >> shadow_ray_epsilon;
    if (XML_PARSE_LOGS)
        cout << "shadow_ray_epsilon = " << shadow_ray_epsilon << endl;

    // Get IntersectionTestEpsilon
    element = root->FirstChildElement("IntersectionTestEpsilon");
    if (element)
    {
        stream << element->GetText() << endl;
    }
    else
    {
        stream << "0.000001" << endl;
    }
    stream >> test_epsilon;
    if (XML_PARSE_LOGS)
        cout << "test_epsilon = " << test_epsilon << endl;

	stream.clear();
    // get max_recursion_depth
    element = root->FirstChildElement("MaxRecursionDepth");
    if (element)
    {
        stream << element->GetText() << endl;
    }
    else
    {
        stream << "0" << endl;
    }
    stream >> max_recursion_depth;
    if (XML_PARSE_LOGS)
        cout << "max_recursion_depth = " << max_recursion_depth << endl;

    float x=0, y=0, z=0;
    // get cameras
    element = root->FirstChildElement("Cameras");
    element = element->FirstChildElement("Camera");
    Camera camera = Camera();
    while (element)
    {
		auto temp = element->Attribute("handedness");
		if (temp && strcmp(temp, "left") == 0)
			camera.is_left_handed = true;

        auto child = element->FirstChildElement("Position");
        stream << child->GetText() << endl;
        child = element->FirstChildElement("Up");
        stream << child->GetText() << endl;
        child = element->FirstChildElement("NearDistance");
        stream << child->GetText() << endl;
        child = element->FirstChildElement("ImageResolution");

        stream << child->GetText() << endl;
        child = element->FirstChildElement("NumSamples");
        if (child)
        {
            stream << child->GetText() << endl;
        }
        else
        {
            stream << "1" << endl;
        }


        child = element->FirstChildElement("ImageName");
        stream << child->GetText() << endl;
        child = element->FirstChildElement("FocusDistance");
        if (child)
        {
            stream << child->GetText() << endl;
        }
        else
        {
            stream << "0" << endl;
        }
        child = element->FirstChildElement("ApertureSize");
        if (child)
        {
            stream << child->GetText() << endl;
        }
        else
        {
            stream << "0" << endl;
        }

        stream >> x >> y >> z;

        camera.position.set_x(x);
        camera.position.set_y(y);
        camera.position.set_z(z);

        stream >> x >> y >> z;

        camera.up.set_x(x);
        camera.up.set_y(y);
        camera.up.set_z(z);

        stream >> camera.near_distance;
        stream >> camera.image_width >> camera.image_height;
		stream >> camera.number_of_samples;
        stream >> camera.image_name;
        stream >> camera.focus_distance;
        stream >> camera.aperture_size;

        if (XML_PARSE_LOGS)
            camera.print_info();

        auto type = element->Attribute("type");
        if (!type)
        {
            child = element->FirstChildElement("Gaze");
            stream << child->GetText() << endl;
            child = element->FirstChildElement("NearPlane");
            stream << child->GetText() << endl;

            stream >> x >> y >> z;

            camera.gaze.set_x(x);
            camera.gaze.set_y(y);
            camera.gaze.set_z(z);

            stream >> camera.near_plane.x >> camera.near_plane.y >> camera.near_plane.z >> camera.near_plane.w;

            camera.left = camera.near_plane.x;
            camera.right = camera.near_plane.y;
            camera.bottom = camera.near_plane.z;
            camera.top = camera.near_plane.w;
        }
        else if (strcmp(type, "lookAt") == 0)
        {
			bool gazeVectorExists = false;
            child = element->FirstChildElement("GazePoint");
            if (!child)
			{
				child = element->FirstChildElement("Gaze");
				gazeVectorExists = true;
			}
            stream << child->GetText() << endl;
            child = element->FirstChildElement("FovY");
            stream << child->GetText() << endl;

            stream >> x >> y >> z;
			if (gazeVectorExists)
			{
                camera.gaze.set_x(x);
                camera.gaze.set_y(y);
                camera.gaze.set_z(z);
			}
			else
			{
                camera.gaze_point.set_x(x);
                camera.gaze_point.set_y(y);
                camera.gaze_point.set_z(z);
			}
            stream >> camera.fov_y;
            camera.calculate_lookat_parameters();
        }

        // get tonemap
        auto tonemapElement = element->FirstChildElement("Tonemap");
        if (tonemapElement)
        {
            string tonemapOperator;
            float key, burn, saturation, gamma;

            child = tonemapElement->FirstChildElement("TMO");
            if (child)
            {
                stream << child->GetText() << endl;
            }
            else
            {
                stream << "Photographic" << endl;
            }

            child = tonemapElement->FirstChildElement("TMOOptions");
            stream << child->GetText() << endl;
            child = tonemapElement->FirstChildElement("Saturation");
            stream << child->GetText() << endl;
            child = tonemapElement->FirstChildElement("Gamma");
            stream << child->GetText() << endl;

            stream >> tonemapOperator;
            stream >> key >> burn >> saturation >> gamma;
            
            Tonemap tonemap = Tonemap(tonemapOperator);
            tonemap.key = key;
            tonemap.burn_ration = burn;
            tonemap.saturation = saturation;
            tonemap.gamma = gamma;

            camera.tonemap = tonemap;
        }

        camera.calculate_parameters();
        v_cameras.push_back(camera);
        element = element->NextSiblingElement("Camera");
    }
	    stream.clear();

    if (XML_PARSE_LOGS)
        for (int i = 0; i < v_cameras.size(); i++)
        {
            cout << "////////////// Camera " << i << "//////////////" << endl;
            v_cameras[i].print_info();
            cout << "//////////////          " << "//////////////" << endl;
        }

    // get lights
    element = root->FirstChildElement("Lights");
    if(element){
        auto child = element->FirstChildElement("AmbientLight");
        
        if(child)
        {
            stream << child->GetText() << endl;

            stream >> x >> y >> z;
            ambient_light.set_x(x);
            ambient_light.set_y(y);
            ambient_light.set_z(z);
            if (XML_PARSE_LOGS)
            {
                cout << "////////////// Ambient Light " << "//////////////" << endl;
                cout << "Light  = " << ambient_light.get_x() << " " << ambient_light.get_y() << " " << ambient_light.get_z()
                    << endl;
                cout << "//////////////          " << "//////////////" << endl;
            }
        }


        element = element->FirstChildElement("AreaLight");
        while (element)
        {
            child = element->FirstChildElement("Position");
            stream << child->GetText() << endl;
            child = element->FirstChildElement("Normal");
            stream << child->GetText() << endl;
            child = element->FirstChildElement("Size");
            stream << child->GetText() << endl;
            child = element->FirstChildElement("Radiance");
            stream << child->GetText() << endl;
            AreaLight area_light = AreaLight();

            stream >> x >> y >> z;
            area_light.get_position().set_x(x);
            area_light.get_position().set_y(y);
            area_light.get_position().set_z(z);
            stream >> x >> y >> z;
            area_light.normal.set_x(x);
            area_light.normal.set_y(y);
            area_light.normal.set_z(z);
            area_light.normal.normalize();

            stream >> area_light.extent;
            stream >> x >> y >> z;
            area_light.radiance.set_x(x);
            area_light.radiance.set_y(y);
            area_light.radiance.set_z(z);
            area_light.calculate_properties();
            area_light.gNURandomDistribution = ref(gNURandomDistribution);
            area_light.gRandomGenerator = ref(gRandomGenerator);

            area_lights.push_back(area_light);
            element = element->NextSiblingElement("AreaLight");
        }
        if (XML_PARSE_LOGS)
            for (int i = 0; i < area_lights.size(); i++)
            {
                cout << "////////////// Area Light " << i << "//////////////" << endl;
                cout << "Light  position = " << area_lights[i].get_position().get_x() << " " << area_lights[i].get_position().get_y()
                        << " " << area_lights[i].get_position().get_z() << endl;
                cout << "Light  normal = " << area_lights[i].normal.get_x() << " "
                        << area_lights[i].normal.get_y() << " " << area_lights[i].normal.get_z() << endl;
                cout << "Light  extent = " << area_lights[i].extent << endl;
                cout << "Light  radiance = " << area_lights[i].radiance.get_x() << " "
                        << area_lights[i].radiance.get_y() << " " << area_lights[i].radiance.get_z() << endl;
                cout << "//////////////          " << "//////////////" << endl;
            }
        for (int i = 0; i < area_lights.size(); i++)
        {
            shared_ptr<Light> raw_pointer = make_shared<AreaLight>(area_lights[i]);
            all_lights.push_back(raw_pointer);
        }
        element = root->FirstChildElement("Lights");
        element = element->FirstChildElement("PointLight");
        PointLight point_light;
        while (element)
        {
            child = element->FirstChildElement("Position");
            stream << child->GetText() << endl;
            child = element->FirstChildElement("Intensity");
            stream << child->GetText() << endl;

            stream >> x >> y >> z;
            point_light.get_position().set_x(x);
            point_light.get_position().set_y(y);
            point_light.get_position().set_z(z);

            stream >> x >> y >> z;
            point_light.intensity.set_x(x);
            point_light.intensity.set_y(y);
            point_light.intensity.set_z(z);

            point_lights.push_back(point_light);
            element = element->NextSiblingElement("PointLight");
        }   
        if (XML_PARSE_LOGS)
            for (int i = 0; i < point_lights.size(); i++)
            {
                cout << "////////////// Point Light " << i << "//////////////" << endl;
                cout << "Light  position = " << point_lights[i].get_position().get_x() << " " << point_lights[i].get_position().get_y()
                        << " " << point_lights[i].get_position().get_z() << endl;
                cout << "Light  intensity = " << point_lights[i].intensity.get_x() << " "
                        << point_lights[i].intensity.get_y() << " " << point_lights[i].intensity.get_z() << endl;
                cout << "//////////////          " << "//////////////" << endl;
            }
        for (int i = 0; i < point_lights.size(); i++)
        {
            shared_ptr<Light> raw_pointer = make_shared<PointLight>(point_lights[i]);
            all_lights.push_back(raw_pointer);
        }
                            

        // get directional lights
        element = root->FirstChildElement("Lights");
        element = element->FirstChildElement("DirectionalLight");
        while (element)
        {
            Vector3 direction, radiance;

            child = element->FirstChildElement("Direction");
            stream << child->GetText() << endl;

            stream >> x >> y >> z;
            direction.set_x(x);
            direction.set_y(y);
            direction.set_z(z);
            
            child = element->FirstChildElement("Radiance");
            stream << child->GetText() << endl;
            stream >> x >> y >> z;
            radiance.set_x(x);
            radiance.set_y(y);
            radiance.set_z(z);

            DirectionalLight dir_light = DirectionalLight();
            dir_light.direction = direction; 
            dir_light.radiance = radiance;

            dir_lights.push_back(dir_light);
            element = element->NextSiblingElement("DirectionalLight");
        }
        for (int i = 0; i < dir_lights.size(); i++)
        {
            shared_ptr<Light> raw_pointer = make_shared<DirectionalLight>(dir_lights[i]);
            all_lights.push_back(raw_pointer);
        }

        // get spot lights
        element = root->FirstChildElement("Lights");
        element = element->FirstChildElement("SpotLight");
        while (element)
        {
            Vector3 position, direction, intensity;
            float coverage_angle, falloff_angle;

            child = element->FirstChildElement("Position");
            stream << child->GetText() << endl;
            
            stream >> x >> y >> z;
            position.set_x(x);
            position.set_y(y);
            position.set_z(z);

            child = element->FirstChildElement("Direction");
            stream << child->GetText() << endl;
            
            stream >> x >> y >> z;
            direction.set_x(x);
            direction.set_y(y);
            direction.set_z(z);

            child = element->FirstChildElement("Intensity");
            stream << child->GetText() << endl;
            
            stream >> x >> y >> z;
            intensity.set_x(x);
            intensity.set_y(y);
            intensity.set_z(z);

            child = element->FirstChildElement("CoverageAngle");
            stream << child->GetText() << endl;
            stream >> coverage_angle;

            child = element->FirstChildElement("FalloffAngle");
            stream << child->GetText() << endl;
            stream >> falloff_angle;

            coverage_angle = (coverage_angle * PI_NUMBER) / 180;
            falloff_angle = (falloff_angle * PI_NUMBER) / 180;

            SpotLight spot_light = SpotLight();

            spot_light.direction = direction; 
            spot_light.intensity = intensity; 
            spot_light.coverage_angle = coverage_angle; 
            spot_light.falloff_angle = falloff_angle;
            spot_light.get_position() = position;

            spot_lights.push_back(spot_light);
            element = element->NextSiblingElement("SpotLight");            
        }
        if (XML_PARSE_LOGS)
            for (int i = 0; i < spot_lights.size(); i++)
            {
                cout << "////////////// Point Light " << i << "//////////////" << endl;
                cout << "Light  position = " << spot_lights[i].get_position().get_x() << " " << spot_lights[i].get_position().get_y()
                        << " " << spot_lights[i].get_position().get_z() << endl;
                cout << "Light  intensity = " << spot_lights[i].intensity.get_x() << " "
                        << spot_lights[i].intensity.get_y() << " " << spot_lights[i].intensity.get_z() << endl;
                cout << "Light  alpha = " << spot_lights[i].coverage_angle << endl;
                cout << "Light  beta = " << spot_lights[i].falloff_angle << endl;
                cout << "//////////////          " << "//////////////" << endl;
            }

        for (int i = 0; i < spot_lights.size(); i++)
        {
            shared_ptr<Light> raw_pointer = make_shared<SpotLight>(spot_lights[i]);
            all_lights.push_back(raw_pointer);
        }
    }
    stream.clear();

	// Get BRDFs
	element = root->FirstChildElement("BRDFs");
	if (element)
	{
		float exponent;
		auto child = element->FirstChildElement("OriginalPhong");
		while (child)
		{
			auto type = child->FirstChildElement("Exponent");
			stream << type->GetText() << endl;
			stream >> exponent;

            shared_ptr<BRDF> brdf = make_shared<PhongBRDF>(exponent);
            brdfs.push_back(brdf);
			child = child->NextSiblingElement("OriginalPhong");
		}

		child = element->FirstChildElement("OriginalBlinnPhong");
		while (child)
		{
			auto type = child->FirstChildElement("Exponent");
			stream << type->GetText() << endl;
			stream >> exponent;

            shared_ptr<BRDF> brdf = make_shared<BlinnPhongBRDF>(exponent);
            brdfs.push_back(brdf);
			child = child->NextSiblingElement("OriginalBlinnPhong");
		}

		child = element->FirstChildElement("ModifiedPhong");
		while (child)
		{
			auto type = child->FirstChildElement("Exponent");
			stream << type->GetText() << endl;
			stream >> exponent;

            bool normalized = false;
            if (child->BoolAttribute("normalized")) 
                normalized = true;

            shared_ptr<BRDF> brdf = make_shared<ModifiedPhongBRDF>(exponent,normalized);
            brdfs.push_back(brdf);
			child = child->NextSiblingElement("ModifiedPhong");
		}

		child = element->FirstChildElement("ModifiedBlinnPhong");
		while (child)
		{
			auto type = child->FirstChildElement("Exponent");
			stream << type->GetText() << endl;
			stream >> exponent;
            
            bool normalized = false;
            if (child->BoolAttribute("normalized")) 
                normalized = true;

            shared_ptr<BRDF> brdf = make_shared<ModifiedBlinnPhongBRDF>(exponent,normalized);
            brdfs.push_back(brdf);
			child = child->NextSiblingElement("ModifiedBlinnPhong");
		}

		child = element->FirstChildElement("TorranceSparrow");
		while (child)
		{
			auto type = child->FirstChildElement("Exponent");
			stream << type->GetText() << endl;
			stream >> exponent;
            
            bool kdfresnel = false;
            if (child->BoolAttribute("kdfresnel")) 
                kdfresnel = true;

            shared_ptr<BRDF> brdf = make_shared<TorranceSparrowBRDF>(exponent,kdfresnel);
            brdfs.push_back(brdf);
			child = child->NextSiblingElement("TorranceSparrow");
		}
	}
    if (XML_PARSE_LOGS)
        for (int i = 0; i < brdfs.size(); i++)
        {
            cout << "////////////// brdfs " << i << "//////////////" << endl;
            cout << "//////////////          " << "//////////////" << endl;
        }

    stream.clear();
    // get materials
    element = root->FirstChildElement("Materials");
    element = element->FirstChildElement("Material");
    Material material;
    while (element)
    {
        if (element->Attribute("type", "mirror") != NULL)
            material.material_type = MIRROR;
        else if (element->Attribute("type", "conductor") != NULL)
            material.material_type = CONDUCTOR;
        else if (element->Attribute("type", "dielectric") != NULL)
            material.material_type = DIELECTRIC;
        else
            material.material_type = NONE;

		auto temp = element->Attribute("degamma");
		if (temp && strcmp(temp, "true") == 0)
		{
			material.degamma = true;
        }
        else
        {
    		material.degamma = false;
        }
        // Get BRDF
		material.brdf_index = element->IntAttribute("BRDF", 0);
		material.brdf_index -= 1;	

        auto child = element->FirstChildElement("AmbientReflectance");
        if (child)
        {
            stream << child->GetText() << endl;
        }
        else
        {
            stream << "0 0 0" << endl;
        }
        child = element->FirstChildElement("DiffuseReflectance");
        if (child)
        {
            stream << child->GetText() << endl;
        }
        else
        {
            stream << "0 0 0" << endl;
        }
        child = element->FirstChildElement("SpecularReflectance");
        if (child)
        {
            stream << child->GetText() << endl;
        }
        else
        {
            stream << "0 0 0" << endl;
        }
        child = element->FirstChildElement("MirrorReflectance");
        if (child)
        {
            stream << child->GetText() << endl;
        }
        else
        {
            stream << "0 0 0" << endl;
        }
        child = element->FirstChildElement("RefractionIndex");
        if (child)
        {
            stream << child->GetText() << endl;
        }
        else
        {
            stream << "0" << endl;
        }
        child = element->FirstChildElement("AbsorptionIndex");
        if (child)
        {
            stream << child->GetText() << endl;
        }
        else
        {
            stream << "0" << endl;
        }
        child = element->FirstChildElement("AbsorptionCoefficient");
        if (child)
        {
            stream << child->GetText() << endl;
        }
        else
        {
            stream << "0 0 0" << endl;
        }
        child = element->FirstChildElement("PhongExponent");
        if (child)
        {
            stream << child->GetText() << endl;
        }
        else
        {
            stream << "1" << endl;
        }
		child = element->FirstChildElement("Roughness");
		if (child)
		{
			stream << child->GetText() << endl;
		}
		else
		{
			stream << "0" << endl;
		}

        stream >> x >> y >> z;
        material.ambient.set_x(x);
        material.ambient.set_y(y);
        material.ambient.set_z(z);

        stream >> x >> y >> z;
        material.diffuse.set_x(x);
        material.diffuse.set_y(y);
        material.diffuse.set_z(z);

        stream >> x >> y >> z;
        material.specular.set_x(x);
        material.specular.set_y(y);
        material.specular.set_z(z);

        stream >> x >> y >> z;
        material.mirror.set_x(x);
        material.mirror.set_y(y);
        material.mirror.set_z(z);

        stream >> x;
        material.refraction_index = x;
        stream >> x;
        material.absorption_index = x;

        stream >> x >> y >> z;
        material.absorption.set_x(x);
        material.absorption.set_y(y);
        material.absorption.set_z(z);

        stream >> material.phong_exponent;
		stream >> material.roughness;

        materials.push_back(material);
        element = element->NextSiblingElement("Material");
    }

    if (XML_PARSE_LOGS)
        for (int i = 0; i < materials.size(); i++)
        {
            cout << "////////////// Materials " << i << "//////////////" << endl;
            cout << "material  material_type = " << materials[i].material_type << endl;
            cout << "material  ambient = " << materials[i].ambient.get_x() << " " << materials[i].ambient.get_y() << " "
                 << materials[i].ambient.get_z() << endl;
            cout << "material  diffuse = " << materials[i].diffuse.get_x() << " " << materials[i].diffuse.get_y() << " "
                 << materials[i].diffuse.get_z() << endl;
            cout << "material  specular = " << materials[i].specular.get_x() << " " << materials[i].specular.get_y()
                 << " " << materials[i].specular.get_z() << endl;
            cout << "material  mirror = " << materials[i].mirror.get_x() << " " << materials[i].mirror.get_y() << " "
                 << materials[i].mirror.get_z() << endl;
            cout << "material  refraction_index = " << materials[i].refraction_index << endl;
            cout << "material  absorption_index = " << materials[i].absorption_index << endl;
            cout << "material  absorption = " << materials[i].absorption.get_x() << " "
                 << materials[i].absorption.get_y() << " " << materials[i].absorption.get_z() << endl;
            cout << "material  phong_exponent = " << materials[i].phong_exponent << endl;
            cout << "//////////////          " << "//////////////" << endl;
        }
        	stream.clear();

	// get textures
	    vector<string> texture_files_paths;
	element = root->FirstChildElement("Textures");
	if (element)
	{
		// images
		string directory(filepath.substr(0, filepath.find_last_of("/") + 1));

		auto child = element->FirstChildElement("Images");
		if (child)
		{
			child = child->FirstChildElement("Image");
			while (child)
			{
				string file_name;
				stream << child->GetText() << endl;
				stream >> file_name;
				string file_path = directory + file_name;
				texture_files_paths.push_back(file_path);
				child = child->NextSiblingElement("Image");
			}
		}

		// texture maps
		element = element->FirstChildElement("TextureMap");
		while (element)
		{
			auto type = element->Attribute("type");
			if (strcmp(type, "image") == 0)
			{
				// image textures
				int img_id;
				string ip_type, decalMode;
				float normalizer, bump_factor;

				child = element->FirstChildElement("ImageId");
				stream << child->GetText() << endl;
				stream >> img_id;
				img_id -= 1;
                
				child = element->FirstChildElement("DecalMode");
				stream << child->GetText() << endl;
				stream >> decalMode;


				child = element->FirstChildElement("BumpFactor");
				if (child)
				{
					stream << child->GetText() << endl;
				}
				else
				{
					stream << "1" << endl;
				}
				stream >> bump_factor;

				child = element->FirstChildElement("Interpolation");
				if (child)
				{
					stream << child->GetText() << endl;
				}
				else
				{
					stream << "bilinear" << endl;
				}
				stream >> ip_type;

				child = element->FirstChildElement("Normalizer");
				if (child)
				{
					stream << child->GetText() << endl;
				}
				else
				{
					stream << "255" << endl;
				}
				stream >> normalizer;

				shared_ptr<ImageTexture> img_texture = make_shared<ImageTexture>(ip_type);
                img_texture->bump_factor = bump_factor;
                img_texture->normalizer = normalizer;
                img_texture->image_path = texture_files_paths[img_id];
				img_texture->calculate_properties(decalMode);
                textures.push_back(img_texture);

				if (img_texture->decal_mode == replace_bg)
				{
					bg_texture = img_texture;
				}
			}
			else if (strcmp(type, "perlin") == 0)
			{
				string decal_mode, noise_conv;
				float noise_scale, bump_factor;

				// perlin textures
				child = element->FirstChildElement("DecalMode");
				stream << child->GetText() << endl;
				stream >> decal_mode;

				child = element->FirstChildElement("NoiseConversion");
				stream << child->GetText() << endl;
				stream >> noise_conv;

				child = element->FirstChildElement("NoiseScale");
				if (child)
				{
					stream << child->GetText() << endl;
				}
				else
				{
					stream << "1" << endl;
				}
				stream >> noise_scale;
				
				child = element->FirstChildElement("BumpFactor");
				if (child)
				{
					stream << child->GetText() << endl;
				}
				else
				{
					stream << "1" << endl;
				}

				stream >> bump_factor;


				shared_ptr<PerlinTexture> perlin_texture = make_shared<PerlinTexture>(noise_conv);
				perlin_texture->scaling_factor = noise_scale;
				perlin_texture->bump_factor = bump_factor;
				perlin_texture->calculate_properties(decal_mode);
                textures.push_back(perlin_texture);
			}
			else if (strcmp(type, "checkerboard") == 0)
			{
                // checkerboard textures
				string decal_mode;
				Vector3 black, white;
				float scale, offset;

				child = element->FirstChildElement("DecalMode");
				stream << child->GetText() << endl;
				stream >> decal_mode;

				child = element->FirstChildElement("BlackColor");
				if (child)
				{
					stream << child->GetText() << endl;
				}
				else
				{
					stream << "0 0 0" << endl;
				}
				stream >> x >> y >> z;
                black.set_x(x);
                black.set_y(y);
                black.set_z(z);

				child = element->FirstChildElement("WhiteColor");
				if (child)
				{
					stream << child->GetText() << endl;
				}
				else
				{
					stream << "255 255 255" << endl;
				}
				stream >> x >> y >> z;
                white.set_x(x);
                white.set_y(y);
                white.set_z(z);

				child = element->FirstChildElement("Offset");
				if (child)
				{
					stream << child->GetText() << endl;
				}
				else
				{
					stream << "0.01" << endl;
				}
				stream >> offset;

				child = element->FirstChildElement("Scale");
				if (child)
				{
					stream << child->GetText() << endl;
				}
				else
				{
					stream << "1" << endl;
				}
				stream >> scale ;

				shared_ptr<CheckerboardTexture> cb_texture = make_shared<CheckerboardTexture>();
                cb_texture->black = black;
                cb_texture->white = white;
                cb_texture->offset = offset;
                cb_texture->scale = scale;

                cb_texture->calculate_properties(decal_mode);
                textures.push_back(cb_texture);
			}

			element = element->NextSiblingElement("TextureMap");
		}
	}
	stream.clear();

	// get spherical directional lights
    element = root->FirstChildElement("Lights");
    if(element){
        element = element->FirstChildElement("SphericalDirectionalLight");
        while (element)
        {
            int id;
            auto child = element->FirstChildElement("ImageId");
            stream << child->GetText() << endl;
            stream >> id;
            id-=1;	

            shared_ptr<Light> sd_light = make_shared<SphericalDirLight>(texture_files_paths[id]);

            all_lights.push_back(sd_light);
            spherical_dir_light  = make_shared<SphericalDirLight>(texture_files_paths[id]);
            if (XML_PARSE_LOGS)
                cout << "////////////// spherical_dir_light //////////////" << endl;
            element = element->NextSiblingElement("SphericalDirectionalLight");
        }    
    }   
	// get transformations
	element = root->FirstChildElement("Transformations");
	if (element)
	{
        float t;
		// translations
		auto child = element->FirstChildElement("Translation");
		while (child)
		{
            Translation translation;
			stream << child->GetText() << endl;

			stream >> x >> y >> z;
            translation.m_translation_vector.set_x(x);
            translation.m_translation_vector.set_y(y);
            translation.m_translation_vector.set_z(z);
            translation.calculate_matrix();
			translations.push_back(translation);

			child = child->NextSiblingElement("Translation");
		}

		// scalings
		child = element->FirstChildElement("Scaling");
		while (child)
		{
			Scaling scaling;
			stream << child->GetText() << endl;

            stream >> x >> y >> z;
            scaling.m_scaling_vector.set_x(x);
            scaling.m_scaling_vector.set_y(y);
            scaling.m_scaling_vector.set_z(z);
            scaling.calculate_matrix();
			scalings.push_back(scaling);

			child = child->NextSiblingElement("Scaling");
		}

		// rotations
		child = element->FirstChildElement("Rotation");
		while (child)
		{
			Rotation rotation;
			stream << child->GetText() << endl;
            stream >> t >> x >> y  >> z;

            //degree to rad
            rotation.m_angle = (t * PI_NUMBER) / 180;
            rotation.m_axis.set_x(x);
            rotation.m_axis.set_y(y);
            rotation.m_axis.set_z(z);
            rotation.calculate_matrix();
			rotations.push_back(rotation);

			child = child->NextSiblingElement("Rotation");
		}
		stream.clear();

		// get composites
		child = element->FirstChildElement("Composite");
		while (child)
		{
			float elements[16];
			stream << child->GetText() << endl;
			for (int i = 0; i < 16; i++)
			{
            	stream >> elements[i];
			}
            Composite composite = Composite();
            composite.calculate(elements);
			composites.push_back(composite);
			child = child->NextSiblingElement("Composite");
		}
	}
	stream.clear();

    if (XML_PARSE_LOGS){
        for (int i = 0; i < translations.size(); i++)
        {
            cout << "////////////// translations " << i << "//////////////" << endl;
            cout << " m_translation_vector = " << translations[i].m_translation_vector << endl;
        }
        for (int i = 0; i < scalings.size(); i++)
        {
            cout << "////////////// scalings " << i << "//////////////" << endl;
            cout << " m_scaling_vector = " << scalings[i].m_scaling_vector << endl;
        }
        for (int i = 0; i < rotations.size(); i++)
        {
            cout << "////////////// rotations " << i << "//////////////" << endl;
            cout << " m_angle = " << rotations[i].m_angle << endl;
            cout << " m_axis = " << rotations[i].m_axis << endl;
        }
        for (int i = 0; i < composites.size(); i++)
        {
            cout << "////////////// composites " << i << "//////////////" << endl;

        }


    }
    // get vertex_data
    element = root->FirstChildElement("VertexData");
    if (element)
	{
        stream << element->GetText() << endl;
        Vertex vertex = Vertex();
        while (!(stream >> x).eof())
        {
            stream >> y >> z;
            vertex.position.set_x(x);
            vertex.position.set_y(y);
            vertex.position.set_z(z);
            vertex_data.push_back(vertex);
        }
	}
    stream.clear();

	// get texture coordinates
	element = root->FirstChildElement("TexCoordData");
	if (element)
	{
		stream << element->GetText() << endl;
		Vector2 coordinates;
		while (!(stream >> coordinates.x).eof())
		{
			stream >> coordinates.y;
			texture_coordinates.push_back(coordinates);
		}
	}
	stream.clear();

    if (XML_PARSE_LOGS)
        for (int i = 0; i < vertex_data.size(); i++)
        {
            cout << "////////////// Vertex " << i + 1 << "//////////////" << endl;
            cout << "vertex  = " << vertex_data[i].position.get_x() << " " << vertex_data[i].position.get_y() << " "
                 << vertex_data[i].position.get_z() << endl;
        }

    // Get Meshes
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Mesh");
    while (element)
    {
        bool smooth = false;
        if (element->Attribute("shadingMode"))
        {
            auto mode = element->Attribute("shadingMode");
            if (strcmp(mode, "smooth") == 0)
                smooth = true;
        }
        Mesh mesh = Mesh(this);

        Matrix4 transformation_matrix = Matrix4(1);
        bool transform = false;
        auto child = element->FirstChildElement("Transformations");
        if (child)
        {
            transform = true;
            vector<char> types;
            vector<int> ids;
            char type = 0;
            int id;
            stream << child->GetText() << endl;
            while (!(stream >> type).eof())
            {
                types.push_back(type);
                stream >> id;
                ids.push_back(id-1);
            }

            transformation_matrix = calculate_transformation_matrix(types, ids, transformation_matrix);
			stream.clear();
        }

        child = element->FirstChildElement("Material");
        stream << child->GetText() << endl;
        stream >> x;
        mesh.material_id = x - 1;

        
        shared_ptr<Texture> texture;
        shared_ptr<Texture> normal_texture;
		child = element->FirstChildElement("Textures");
		if (child)
		{
			int tex_id = -1;
			int normal_tex_id = -1;
			stream << child->GetText() << endl;
			stream >> tex_id ;
			stream >> normal_tex_id;
			tex_id -= 1;
			normal_tex_id -= 1;
            if (normal_tex_id < 0)
			{
				if (textures[tex_id]->is_normal || textures[tex_id]->is_bump)
					normal_texture = textures[tex_id];
				else
					texture = textures[tex_id];
			}
			else
			{
				texture = textures[tex_id]; //shading
				normal_texture = textures[normal_tex_id]; // normal texture
			}
			 
		}
		stream.clear();
        mesh.texture = texture;
        mesh.np_texture = normal_texture;
		bool motion_blur = false;
		Vector3 motion_blur_vector = Vector3();
		child = element->FirstChildElement("MotionBlur");
		if (child)
		{
			motion_blur = true;
			stream << child->GetText() << endl;
            stream >> x >> y >> z;
            motion_blur_vector.set_x(x);
            motion_blur_vector.set_y(y);
            motion_blur_vector.set_z(z);
		}

        child = element->FirstChildElement("Faces");
        auto temp = element->FirstChildElement("Faces");
        stream << child->GetText() << endl;
                
        int vertex_offset ;
        int tex_offset ;
        vertex_offset = child->IntAttribute("vertexOffset", 0);
        tex_offset = child->IntAttribute("textureOffset", 0);

        vector<shared_ptr<Shape>> mesh_triangles;
        const char *ply_file = temp->Attribute("plyFile");
        if (ply_file)
        {
            if (XML_PARSE_LOGS)
                cout << "////////////// Mesh plyFile =  " << ply_file << "//////////////" << endl;
            ply_parser(filepath,ply_file, mesh, smooth, vertex_offset , tex_offset);
        }
        else
        {
            int v0, v1, v2;

            while (!(stream >> v0).eof())
            {
                shared_ptr<Triangle> triangle = make_shared<Triangle>(this);
                triangle->material_id = mesh.material_id;
                triangle->v_offset = vertex_offset;
                triangle->t_offset = tex_offset;
                triangle->texture = texture;
                triangle->np_texture = normal_texture;
                stream >> v1 >> v2;
				v0--,v1--,v2--;
                triangle->vertex_indices.v0 = v0 + vertex_offset;
                triangle->vertex_indices.v1 = v1 + vertex_offset;
                triangle->vertex_indices.v2 = v2 + vertex_offset;

                triangle->texture_indices.v0 = v0 + tex_offset;
                triangle->texture_indices.v1 = v1 + tex_offset;
                triangle->texture_indices.v2 = v2 + tex_offset;

                triangle->calculate_properties();
                if (smooth)
                {
                    vertex_data[v0 ].calculate_shading_properties(triangle->normal);
                    vertex_data[v1 ].calculate_shading_properties(triangle->normal);
                    vertex_data[v2 ].calculate_shading_properties(triangle->normal);
                    triangle->smooth = true;
                }
                mesh.triangles.push_back(triangle);
            }
        }
        mesh.transform = transform;
        mesh.transformation_matrix = transformation_matrix;
        mesh.motion_blur = motion_blur;
        mesh.motion_blur_vector = motion_blur_vector;
        mesh.calculate_properties();		
        mesh.calculate_matrices();

        meshes.push_back(mesh);
        stream.clear();

        mesh.triangles.clear();
        element = element->NextSiblingElement("Mesh");
    }
    stream.clear();
    for (int i = 0; i < meshes.size(); i++)
    {
        shared_ptr<Mesh> temp_mesh = make_shared<Mesh>(meshes[i]);
        
        p_all_shapes.push_back(temp_mesh);
        base_meshes.push_back(temp_mesh);
    }

    if (XML_PARSE_LOGS)
        for (int i = 0; i < meshes.size(); i++)
        {
            cout << "////////////// Mesh " << i << "//////////////" << endl;
            meshes[i].print_info();
            cout << "//////////////          " << "//////////////" << endl;
        }
        
	// vertex_data normals normalization
    for (int i = 0; i < vertex_data.size(); i++)
    {
	 	vertex_data[i].normal /= vertex_data[i].num_triangles;
	 	vertex_data[i].normal = vertex_data[i].normal.unit_vector();
	}

    // Get Mesh Instances
	element = root->FirstChildElement("Objects");
	element = element->FirstChildElement("MeshInstance");
	while (element)
	{
		int baseMeshId = atoi(element->Attribute("baseMeshId"));
		baseMeshId--;	// convert to 0-based index
		shared_ptr<Shape> baseMesh = base_meshes[baseMeshId];
		const char* reset_tranform = element->Attribute("resetTransform");

		int materialId;
		auto child = element->FirstChildElement("Material");
		stream << child->GetText() << endl;
		stream >> materialId;
		materialId-=1;	// convert to 0-based index

        shared_ptr<MeshInstance> mesh_instance = make_shared<MeshInstance>();

		shared_ptr<Texture> texture;
		shared_ptr<Texture> normal_texture;
		child = element->FirstChildElement("Textures");
		if (child)
		{
			int tex_id = -1;
			int normal_tex_id = -1;
			stream << child->GetText() << endl;
			stream >> tex_id ;
			stream >> normal_tex_id;
			tex_id -= 1;
			normal_tex_id -= 1;
            if (normal_tex_id < 0)
			{
				if (textures[tex_id]->is_normal || textures[tex_id]->is_bump)
					normal_texture = textures[tex_id];
				else
					texture = textures[tex_id];
			}
			else
			{
				texture = textures[tex_id]; //shading
				normal_texture = textures[normal_tex_id]; // normal texture
			}
			 
		}
		stream.clear();

        mesh_instance->texture = texture;
        mesh_instance->np_texture = normal_texture;

		bool transform = false;
		Matrix4 transformation_matrix = Matrix4(1);
		child = element->FirstChildElement("Transformations");
		if (child)
		{
			transform = true;            
            vector<char> types;
            vector<int> ids;
            char type = 0;
            int id;
            stream << child->GetText() << endl;
            while (!(stream >> type).eof())
            {
                types.push_back(type);
                stream >> id;
                ids.push_back(id-1);
            }
			if (reset_tranform && strcmp(reset_tranform, "true") == 0)
			{
                transformation_matrix = calculate_transformation_matrix(types, ids, transformation_matrix);
			}
			else
			{
				transformation_matrix = baseMesh->transformation_matrix;
                transformation_matrix = calculate_transformation_matrix(types, ids, transformation_matrix);
			}			
		}
		stream.clear();
		bool motionBlur = false;
		Vector3 motion_blur_vector = Vector3(0);
		child = element->FirstChildElement("MotionBlur");
		if (child)
		{
			motionBlur = true;
			stream << child->GetText() << endl;
			stream >> x >> y >> z;
            motion_blur_vector.set_x(x);
            motion_blur_vector.set_y(y);
            motion_blur_vector.set_z(z);
		}

        //cout << "   Mesh Instances bvh "<< p_bvh << endl;
        mesh_instance->p_scene = this;
        mesh_instance->material_id = materialId;
        mesh_instance->transformation_matrix = transformation_matrix;
        mesh_instance->transform = transform;
		mesh_instance->motion_blur_vector = motion_blur_vector;
        mesh_instance->motion_blur = motionBlur;
        mesh_instance->baseMeshBVH = baseMesh->get_bvh();
	    mesh_instance->bounding_box = baseMesh->get_bounding_box();
        mesh_instance->calculate_properties();

		p_all_shapes.push_back(mesh_instance);
        base_meshes.push_back(mesh_instance);
		element = element->NextSiblingElement("MeshInstance");
        stream.clear(); 
	}
    stream.clear();

	// Get LightMeshes
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("LightMesh");
    while (element)
    {
        bool smooth = false;


        Mesh mesh = Mesh(this);

        Matrix4 transformation_matrix = Matrix4(1);
        bool transform = false;
        auto child = element->FirstChildElement("Transformations");
        if (child)
        {
            transform = true;
            vector<char> types;
            vector<int> ids;
            char type = 0;
            int id;
            stream << child->GetText() << endl;
            while (!(stream >> type).eof())
            {
                types.push_back(type);
                stream >> id;
                ids.push_back(id-1);
            }

            transformation_matrix = calculate_transformation_matrix(types, ids, transformation_matrix);
			stream.clear();
        }

        child = element->FirstChildElement("Material");
        stream << child->GetText() << endl;
        stream >> x;
        int material_id = x - 1;

        shared_ptr<Texture> texture;
        shared_ptr<Texture> normal_texture;

	    double x,y,z;
		Vector3 radiance = Vector3();;
		child = element->FirstChildElement("Radiance");
		stream << child->GetText() << endl;
		stream >> x >> y >> z;
		radiance.set_x(x);
		radiance.set_y(y);
		radiance.set_z(z);

		bool motion_blur = false;
		Vector3 motion_blur_vector = Vector3();
		child = element->FirstChildElement("MotionBlur");
		if (child)
		{
			motion_blur = true;
			stream << child->GetText() << endl;
            stream >> x >> y >> z;
            motion_blur_vector.set_x(x);
            motion_blur_vector.set_y(y);
            motion_blur_vector.set_z(z);
		}

        child = element->FirstChildElement("Faces");
        auto temp = element->FirstChildElement("Faces");
        stream << child->GetText() << endl;
                
        int vertex_offset ;
        int tex_offset ;
        vertex_offset = child->IntAttribute("vertexOffset", 0);
        tex_offset = child->IntAttribute("textureOffset", 0);

        vector<shared_ptr<Shape>> mesh_triangles;
        const char *ply_file = temp->Attribute("plyFile");
        if (ply_file)
        {
            if (XML_PARSE_LOGS)
                cout << "////////////// Mesh plyFile =  " << ply_file << "//////////////" << endl;
            ply_parser(filepath,ply_file, mesh, smooth, vertex_offset , tex_offset);
        }
        else
        {
            int v0, v1, v2;

            while (!(stream >> v0).eof())
            {
                shared_ptr<Triangle> triangle = make_shared<Triangle>(this);
                triangle->material_id = mesh.material_id;
                triangle->v_offset = vertex_offset;
                triangle->t_offset = tex_offset;
                triangle->texture = texture;
                triangle->np_texture = normal_texture;
                stream >> v1 >> v2;
                triangle->vertex_indices.v0 = v0 + vertex_offset - 1;
                triangle->vertex_indices.v1 = v1 + vertex_offset - 1;
                triangle->vertex_indices.v2 = v2 + vertex_offset - 1;

                triangle->texture_indices.v0 = v0 + tex_offset - 1;
                triangle->texture_indices.v1 = v1 + tex_offset - 1;
                triangle->texture_indices.v2 = v2 + tex_offset - 1;

                triangle->calculate_properties();

                mesh_triangles.push_back(triangle);
            }
        }    
				cout << "triangles.size = "<< mesh_triangles.size() << endl;

        shared_ptr<LightMesh> light_mesh = make_shared<LightMesh>(this);
        light_mesh->radiance = radiance;
        light_mesh->triangles = mesh_triangles;
        light_mesh->material_id = material_id;
        light_mesh->transform = transform;
        light_mesh->motion_blur = motion_blur;
        light_mesh->transformation_matrix = transformation_matrix;
        light_mesh->motion_blur_vector = motion_blur_vector;
        light_mesh->texture = texture;
        light_mesh->np_texture = normal_texture;	
        light_mesh->calculate_properties();		
        light_mesh->calculate_matrices();
	    light_mesh->calculate_CDF();

        all_lights.push_back(light_mesh);
        p_all_shapes.push_back(light_mesh);
        stream.clear();
        element = element->NextSiblingElement("LightMesh");

    }    
    stream.clear();
    
    // Get Triangles
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Triangle");
    while (element)
    {
        Triangle triangle = Triangle(this);
        auto child = element->FirstChildElement("Material");
        stream << child->GetText() << endl;
        stream >> x;
        triangle.material_id = x - 1;

		shared_ptr<Texture> texture;
		shared_ptr<Texture> normal_texture;
		child = element->FirstChildElement("Textures");
		if (child)
		{
			int tex_id = -1;
			int normal_tex_id = -1;
			stream << child->GetText() << endl;
			stream >> tex_id ;
			stream >> normal_tex_id;
			tex_id -= 1;
			normal_tex_id -= 1;
            if (normal_tex_id < 0)
			{
				if (textures[tex_id]->is_normal || textures[tex_id]->is_bump)
					normal_texture = textures[tex_id];
				else
					texture = textures[tex_id];
			}
			else
			{
				texture = textures[tex_id]; //shading
				normal_texture = textures[normal_tex_id]; // normal texture
			}
			 
		}
		stream.clear();


        triangle.texture = texture;
        triangle.np_texture = normal_texture;


        child = element->FirstChildElement("Indices");
        stream << child->GetText() << endl;
        int v0, v1, v2;
        stream >> v0 >> v1 >> v2;

        triangle.vertex_indices.v0 = v0 - 1;
        triangle.vertex_indices.v1 = v1 - 1;
        triangle.vertex_indices.v2 = v2 - 1;

        triangle.texture_indices.v0 = v0 - 1;
        triangle.texture_indices.v1 = v1 - 1;
        triangle.texture_indices.v2 = v2 - 1;

        triangle.calculate_properties();

        Matrix4 transformation_matrix = Matrix4(1);
        bool transform = false;
        child = element->FirstChildElement("Transformations");
        if (child)
        {
            transform = true;
            vector<char> types;
            vector<int> ids;
            char type;
            int id;
            while (stream >> type)
            {
                types.push_back(type);
                stream >> id;
                ids.push_back(id-1);
            }

            transformation_matrix = calculate_transformation_matrix(types, ids,transformation_matrix);
        }
        triangle.transform = transform;
        triangle.transformation_matrix = transformation_matrix;

		bool motion_blur = false;
		Vector3 motion_blur_vector = Vector3();
		child = element->FirstChildElement("MotionBlur");
		if (child)
		{
			motion_blur = true;
			stream << child->GetText() << endl;
            stream >> x >> y >> z;
            motion_blur_vector.set_x(x);
            motion_blur_vector.set_y(y);
            motion_blur_vector.set_z(z);
		}

        triangle.motion_blur = motion_blur;
        triangle.motion_blur_vector = motion_blur_vector;

        triangles.push_back(triangle);
        element = element->NextSiblingElement("Triangle");
    }
    for (int i = 0; i < triangles.size(); i++)
    {        
        shared_ptr<Triangle> temp_triangle = make_shared<Triangle>(triangles[i]);
        p_all_shapes.push_back(temp_triangle);
    }
    if (XML_PARSE_LOGS)
        for (int i = 0; i < triangles.size(); i++)
        {
            cout << "////////////// Triangle " << i << "//////////////" << endl;
            triangles[i].print_info();
            cout << "//////////////          " << "//////////////" << endl;
        }
    // Get Spheres
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Sphere");
    while (element)
    {
        stream.clear();
        Sphere sphere = Sphere(this);
        auto child = element->FirstChildElement("Material");
        stream << child->GetText() << endl;

        stream >> x;
        sphere.material_id = x - 1;
        
		shared_ptr<Texture> texture;
		shared_ptr<Texture> normal_texture;
		child = element->FirstChildElement("Textures");
		if (child)
		{
			int tex_id = -1;
			int normal_tex_id = -1;
			stream << child->GetText() << endl;
			stream >> tex_id ;
			stream >> normal_tex_id;
			tex_id -= 1;
			normal_tex_id -= 1;
            if (normal_tex_id < 0)
			{
				if (textures[tex_id]->is_normal || textures[tex_id]->is_bump)
					normal_texture = textures[tex_id];
				else
					texture = textures[tex_id];
			}
			else
			{
				texture = textures[tex_id]; //shading
				normal_texture = textures[normal_tex_id]; // normal texture
			}
			 
		}
		stream.clear();

        sphere.texture = texture;
        sphere.np_texture = normal_texture;


        child = element->FirstChildElement("Center");
        stream << child->GetText() << endl;
        stream >> x;
        sphere.center_vertex_id = x - 1;

        child = element->FirstChildElement("Radius");
        stream << child->GetText() << endl;
        stream >> sphere.radius;

        
        Matrix4 transformation_matrix = Matrix4(1);
        bool transform = false;
        child = element->FirstChildElement("Transformations");
        if (child)
        {
			transform = true;            
            vector<char> types;
            vector<int> ids;
            char type = 0;
            int id;
            stream << child->GetText() << endl;
            while (!(stream >> type).eof())
            {
                types.push_back(type);
                stream >> id;
                ids.push_back(id-1);
            }
            transformation_matrix = calculate_transformation_matrix(types, ids, transformation_matrix);
        }
        sphere.transform = transform;
        sphere.transformation_matrix = transformation_matrix;

		bool motion_blur = false;
		Vector3 motion_blur_vector = Vector3();
		child = element->FirstChildElement("MotionBlur");
		if (child)
		{
			motion_blur = true;
			stream << child->GetText() << endl;
            stream >> x >> y >> z;
            motion_blur_vector.set_x(x);
            motion_blur_vector.set_y(y);
            motion_blur_vector.set_z(z);
		}

        sphere.motion_blur = motion_blur;
        sphere.motion_blur_vector = motion_blur_vector;

        sphere.calculate_properties();
        spheres.push_back(sphere);
        element = element->NextSiblingElement("Sphere");
    }
    for (int i = 0; i < spheres.size(); i++)
    {
        shared_ptr<Sphere> temp_sphere = make_shared<Sphere>(spheres[i]);
        p_all_shapes.push_back(temp_sphere);
    }
    if (XML_PARSE_LOGS)
        for (int i = 0; i < spheres.size(); i++)
        {
            cout << "////////////// Sphere " << i << "//////////////" << endl;
            spheres[i].print_info();
            cout << "//////////////          " << "//////////////" << endl;
        }

   	// Get LightSpheres
	element = root->FirstChildElement("Objects");
	element = element->FirstChildElement("LightSphere");
	while (element)
	{
		int materialId;
		auto child = element->FirstChildElement("Material");
		stream << child->GetText() << endl;
		stream >> materialId;
		materialId-=1;

        shared_ptr<Texture> texture;
        shared_ptr<Texture> normal_texture;

		Vector3 radiance;
		child = element->FirstChildElement("Radiance");
		stream << child->GetText() << endl;
        stream >> x >> y >> z;
        radiance.set_x(x);
        radiance.set_y(y);
        radiance.set_z(z);

		int centerVertexId;
		child = element->FirstChildElement("Center");
		stream << child->GetText() << endl;
		stream >> centerVertexId;
		centerVertexId-=1;

		float radius;
		child = element->FirstChildElement("Radius");
		stream << child->GetText() << endl;
		stream >> radius;

		bool transform = false;
		Matrix4 transformationMat = Matrix4(1);
		child = element->FirstChildElement("Transformations");
		if (child)
		{
			transform = true;            
            vector<char> types;
            vector<int> ids;
            char type = 0;
            int id;
            stream << child->GetText() << endl;
            while (!(stream >> type).eof())
            {
                types.push_back(type);
                stream >> id;
                ids.push_back(id-1);
            }
            transformationMat = calculate_transformation_matrix(types, ids, transformationMat);
		}

		bool motionBlur = false;
		Vector3 motionVec = Vector3(0, 0, 0);
		child = element->FirstChildElement("MotionBlur");
		if (child)
		{
			motionBlur = true;
			stream << child->GetText() << endl;
			stream >> x >> y >> z;
            motionVec.set_x(x);
            motionVec.set_y(y);
            motionVec.set_z(z);
		}
        
		shared_ptr<LightSphere> light_sphere = make_shared<LightSphere>(this);
		
        light_sphere->center_vertex_id = centerVertexId;
        light_sphere->radius = radius;
        light_sphere->transform = transform;
        light_sphere->motion_blur = motionBlur;
        light_sphere->material_id = materialId;
        light_sphere->transformation_matrix = transformationMat;
        light_sphere->motion_blur_vector = motionVec;
        light_sphere->texture = texture;
        light_sphere->np_texture = normal_texture;
        light_sphere->radiance= radiance;
        light_sphere->calculate_properties();

        all_lights.push_back(light_sphere);
		p_all_shapes.push_back(light_sphere);
		element = element->NextSiblingElement("LightSphere");
	}
    p_bounding_volume = new BVH(p_all_shapes, 0, 0, p_all_shapes.size());
}
/**
 * Processes the scene to create image for each camera.
 */
void Scene::process_scene()
{
    for (Camera camera : v_cameras)
    {
        int image_width= camera.image_width;
        int image_height= camera.image_height;

        // create image object
		vector<Color> a_pixelColors(image_width * image_height, Color());
        pixelColors = a_pixelColors;

        // create SDL window
        program_running = true;
        std::thread t(&Scene::SDL_thread_start, this,image_width, image_height, camera.image_name);
        SDL_Delay(2000);

        // get the current time before the scene process
        auto start_time = chrono::high_resolution_clock::now();

        int square_edge = 20;

        task_thread_pool::task_thread_pool pool(20);

        // divide image into squares and enqueue
        for (int i = 0; i < image_height; i+=square_edge)
        {        for (int j = 0; j < image_width; j+=square_edge)
            {
                int start_row = i;
                int end_row = (i+square_edge < image_height ) ? (i+square_edge) : image_height;
                int start_col = j;
                int end_col = (j+square_edge < image_width ) ? (j+square_edge) : image_width;

                pool.submit_detach([this,start_row, end_row, start_col, end_col,image_width, &camera]
                {this->render_chunk(start_row, end_row, start_col, end_col, image_width,  camera);});
            }
        }

        pool.wait_for_tasks();

        Image image(pixelColors, image_width, image_height);
        image.save_image(camera.image_name,camera.tonemap);

        // gets the current time after the scene process
        auto end_time = chrono::high_resolution_clock::now();

        // calculates the total duration
        chrono::duration<double> duration = end_time - start_time;

        // prints the duration in seconds
        cout << "Rendering Time of " << camera.image_name << ": " << duration.count() << " seconds" << endl;
    
        program_running = false;
        t.join();

    }
}

/**
 * Parses the ply file. It simply parses the meshes from the ply file into the
 * data structure inside the program.
 *
 * @param a_ply_file path and name of ply file
 * @param vertex_data contains all the vertex in the scene
 * @param meshes contains all the meshes in the scene
 */
void Scene::ply_parser(const string& filepath,const char *a_ply_file, Mesh &mesh, bool smooth,int vertex_offset,int texture_offset)
{
    int pos = filepath.find_last_of("/");
	string plyDir(filepath.substr(0, pos + 1));
	string plyPath = plyDir + a_ply_file;

    happly::PLYData plyIn(plyPath);

    int number_of_vertex_indices = vertex_data.size();
    vector<array<double, 3>> vPos = plyIn.getVertexPositions();

    for (int i = 0; i < vPos.size(); i++)
    {
        Vertex vertex = Vertex();
        vertex.position.set_x(vPos[i][0]);
        vertex.position.set_y(vPos[i][1]);
        vertex.position.set_z(vPos[i][2]);
        vertex_data.push_back(vertex);
    }

    bool uvExists = plyIn.getElement("vertex").hasProperty("u");
    vector<double> u_position, v_position;

    if (uvExists)
    {
        v_position = plyIn.getElement("vertex").getProperty<double>("v");
        u_position = plyIn.getElement("vertex").getProperty<double>("u");
    }	  

    vector<array<double, 2>> uv_texture(u_position.size());
    for (int i = 0; i < uv_texture.size(); i++)
    {
        uv_texture[i][0] = u_position[i];
        uv_texture[i][1] = v_position[i];
    }

	int number_of_texture_indices = texture_coordinates.size();
	for (int i = 0; i < uv_texture.size(); i++)
	{
		Vector2 uv = Vector2(uv_texture[i][0], uv_texture[i][1]);
		texture_coordinates.push_back(uv);
	}
	texture_offset += number_of_texture_indices;
	vertex_offset += number_of_vertex_indices;

    vector<vector<size_t>> f_ind = plyIn.getFaceIndices<size_t>();

    for (int i = 0; i < f_ind.size(); i++)
    {
        if (f_ind[i].size() == 3)
        {
            shared_ptr<Triangle> triangle1 = make_shared<Triangle>(this);
            triangle1->vertex_indices.v0 = f_ind[i][0] + vertex_offset;//
            triangle1->vertex_indices.v1 = f_ind[i][1] + vertex_offset;
            triangle1->vertex_indices.v2 = f_ind[i][2] + vertex_offset;
            triangle1->texture_indices.v0 = f_ind[i][0] + texture_offset;
            triangle1->texture_indices.v1 = f_ind[i][1] + texture_offset;
            triangle1->texture_indices.v2 = f_ind[i][2] + texture_offset;
            triangle1->transform = false;
            triangle1->transformation_matrix = Matrix4(1);
            triangle1->motion_blur = false;
            triangle1->motion_blur_vector = Vector3(0);
            triangle1->material_id = mesh.material_id;
            triangle1->texture = mesh.texture;
            triangle1->np_texture = mesh.np_texture;
            triangle1->calculate_properties();

            if (smooth)
            {
                vertex_data[triangle1->vertex_indices.v0].calculate_shading_properties(triangle1->normal);
                vertex_data[triangle1->vertex_indices.v1].calculate_shading_properties(triangle1->normal);
                vertex_data[triangle1->vertex_indices.v2].calculate_shading_properties(triangle1->normal);
                triangle1->smooth = true;
            }
            mesh.triangles.push_back(triangle1);
        }
        else if (f_ind[i].size() == 4)
        {
            shared_ptr<Triangle> triangle1 = make_shared<Triangle>(this);
            triangle1->vertex_indices.v0 = f_ind[i][0] + vertex_offset;
            triangle1->vertex_indices.v1 = f_ind[i][1] + vertex_offset;
            triangle1->vertex_indices.v2 = f_ind[i][2] + vertex_offset;
            triangle1->texture_indices.v0 = f_ind[i][0] + texture_offset;
            triangle1->texture_indices.v1 = f_ind[i][1] + texture_offset;
            triangle1->texture_indices.v2 = f_ind[i][2] + texture_offset;

            triangle1->transform = false;
            triangle1->transformation_matrix = Matrix4(1);
            triangle1->motion_blur = false;
            triangle1->motion_blur_vector = Vector3(0);
            triangle1->material_id = mesh.material_id;
            triangle1->texture = mesh.texture;
            triangle1->np_texture = mesh.np_texture;
            triangle1->calculate_properties();

            shared_ptr<Triangle> triangle2 = make_shared<Triangle>(this);
            triangle2->vertex_indices.v0 = f_ind[i][0] + vertex_offset;
            triangle2->vertex_indices.v1 = f_ind[i][2] + vertex_offset;
            triangle2->vertex_indices.v2 = f_ind[i][3] + vertex_offset;
            triangle2->texture_indices.v0 = f_ind[i][0] + texture_offset;
            triangle2->texture_indices.v1 = f_ind[i][2] + texture_offset;
            triangle2->texture_indices.v2 = f_ind[i][3] + texture_offset;

            triangle2->transform = false;
            triangle2->transformation_matrix = Matrix4(1);
            triangle2->motion_blur = false;
            triangle2->motion_blur_vector = Vector3(0);
            triangle2->material_id = mesh.material_id;
            triangle2->texture = mesh.texture;
            triangle2->np_texture = mesh.np_texture;
            triangle2->calculate_properties();

            if (smooth)
            {
                vertex_data[triangle1->vertex_indices.v0].calculate_shading_properties(triangle1->normal);
                vertex_data[triangle1->vertex_indices.v1].calculate_shading_properties(triangle1->normal);
                vertex_data[triangle1->vertex_indices.v2].calculate_shading_properties(triangle1->normal);

                vertex_data[triangle2->vertex_indices.v0].calculate_shading_properties(triangle2->normal);
                vertex_data[triangle2->vertex_indices.v1].calculate_shading_properties(triangle2->normal);
                vertex_data[triangle2->vertex_indices.v2].calculate_shading_properties(triangle2->normal);
                triangle1->smooth = true;
                triangle2->smooth = true;
            }

            mesh.triangles.push_back(triangle1);
            mesh.triangles.push_back(triangle2);
        }
        
    }
}

Matrix4 Scene::calculate_transformation_matrix(const vector<char> types,const vector<int> ids, Matrix4 transformation_matrix)
{
    int index = 0;
    for (const auto& type : types) {
        switch (type) {
            case 't':
                transformation_matrix = translations[ids[index]].get_transformation_matrix() * transformation_matrix;
                break;
            case 's':
                transformation_matrix = scalings[ids[index]].get_transformation_matrix() * transformation_matrix;
                break;
            case 'r':
                transformation_matrix = rotations[ids[index]].get_transformation_matrix() * transformation_matrix;
                break;
            case 'c':
                transformation_matrix = composites[ids[index]].get_transformation_matrix() * transformation_matrix;
                break;
            default:
                break;
        }
        index++;
    }
    return transformation_matrix;
}

Vector3 Scene::calculate_irradiance(PointLight &light,const Vector3 &intersection_point)
{
    Vector3 irradiance;
    float light_distance_sqr = (light.get_position() - intersection_point).length_sqr();

    if (light_distance_sqr != 0)
        irradiance = light.intensity / light_distance_sqr;

    return irradiance;
}

/**
 * Calculates the diffuse and specular components of the color.
 *
 * @param camera
 * @param material
 * @param light
 * @param intersection_point
 * @param irradiance
 * @param normal
 */
Vector3 Scene::calculate_light_shadings(const Camera &camera,const Material &material, const Vector3& irradiance, Vector3& incoming_dir_vector, Vector3 &intersection_point,const Vector3 &normal, Vector2 uvTexture,  shared_ptr<Texture> a_texture)
{
	Vector3 kd = material.diffuse;
	Vector3 ks = material.specular;

    if (a_texture && a_texture->is_normal == false)
	{
		Color textureColor = a_texture->calculate_texture_color(intersection_point, uvTexture);
        Vector3 color_vector;
        color_vector.set_x(textureColor.red);
        color_vector.set_y(textureColor.green);
        color_vector.set_z(textureColor.blue);

		if (a_texture->decal_mode == replace_kd)
		{
			kd = color_vector;
		}
		else if (a_texture->decal_mode == blend_kd)
		{
            kd += color_vector;
            kd /= 2;
		}
	}

    float cosine_theta = max(float(0), incoming_dir_vector.dot_product(normal));

    Vector3 diffuse_component = (kd * cosine_theta) * irradiance;

    Vector3 outgoing_dir_vector = (camera.position - intersection_point);
    outgoing_dir_vector.normalize();

    float cosine_alpha = max(float(0), normal.dot_product((incoming_dir_vector + outgoing_dir_vector).unit_vector()));

    Vector3 specular_component = ks * powf(cosine_alpha, material.phong_exponent) * irradiance;

    return (diffuse_component + specular_component);
}

Vector3 Scene::calculate_roughness(Vector3 normal, Material material)
{
    // Construct orthonormal basis uvr.
    Vector3 temp_normal = normal;
    int smallest_index = temp_normal.get_smallest_index();
    if(smallest_index == 0)
        temp_normal.set_x(1);
    else if(smallest_index == 1)
        temp_normal.set_y(1);
    else if(smallest_index == 2)
        temp_normal.set_z(1);

    Vector3 u = normal.cross_product(temp_normal);
    Vector3 v = normal.cross_product(u);
    u.normalize();
    v.normalize();

    float u_random = gNURandomDistribution(gRandomGenerator);
    float v_random = gNURandomDistribution(gRandomGenerator);
    normal +=  (v_random * v + u_random * u) * material.roughness ;

    return normal;
}

Vector3 Scene::ray_refraction(Vector3 direction, Vector3 normal, float ref_index,
                              float new_ref_index)
{
    float cosine_angle = -1 * direction.dot_product(normal);
    float term = 1 - pow((ref_index / new_ref_index), 2) * (1 - pow(cosine_angle, 2));
    Vector3 transmission_direction;
    if (term >= 0)
    {
        float cosine_angle2 = sqrt(term);
        transmission_direction = ((direction + normal * cosine_angle) * (ref_index / new_ref_index) -
                                  normal * cosine_angle2);
                                     
        transmission_direction.normalize();
    }
    return transmission_direction;
}

void Scene::calculate_degamma(Material& material, const Tonemap& tonemap)
{
	if (tonemap.type != none && material.degamma == true)
	{
		material.ambient.set_x(pow(material.ambient.get_x(),tonemap.gamma));
		material.ambient.set_y(pow(material.ambient.get_y(),tonemap.gamma));
		material.ambient.set_z(pow(material.ambient.get_z(),tonemap.gamma));

		material.diffuse.set_x(pow(material.diffuse.get_x(),tonemap.gamma));
		material.diffuse.set_y(pow(material.diffuse.get_y(),tonemap.gamma));
		material.diffuse.set_z(pow(material.diffuse.get_z(),tonemap.gamma));

		material.specular.set_x(pow(material.specular.get_x(),tonemap.gamma));
		material.specular.set_y(pow(material.specular.get_y(),tonemap.gamma));
		material.specular.set_z(pow(material.specular.get_z(),tonemap.gamma));

		material.mirror.set_x(pow(material.mirror.get_x(),tonemap.gamma));
		material.mirror.set_y(pow(material.mirror.get_y(),tonemap.gamma));
		material.mirror.set_z(pow(material.mirror.get_z(),tonemap.gamma));
	}
}
/**
 * Finds the color of the pixel in which the ray is sent.
 *
 * @param camera
 * @param ray
 */
Color Scene::find_color(Camera &camera, Ray &ray, int recursion_depth, int i, int j)
{
    Color pixel_color;
    Intersection intersection = Intersection();
    if (BVH_ON)
    {
        intersection = p_bounding_volume->intersect(ray);
        ray.intersection = intersection;
    }
    else
    {
        trace_ray(ray);
    }

    if (!(ray.intersection.does_hit))
    {
        // sets color to background color if there is no intersection
        pixel_color = calculate_bg_pixel_color(ray, i,j);
    }
    else
    {
		if (ray.intersection.is_light_shape == true)
		{
			Vector3 color_vec = ray.intersection.radiance;

            pixel_color.red = color_vec.get_x();
            pixel_color.green = color_vec.get_y();
            pixel_color.blue = color_vec.get_z();
            
            return pixel_color;
		}

        // adds the first component of color which is the ambient light combined
        // with the material ambient
        Material material = materials[ray.intersection.material_id];
        pixel_color.red = (ambient_light * material.ambient).get_x();
        pixel_color.green = (ambient_light * material.ambient).get_y();
        pixel_color.blue = (ambient_light * material.ambient).get_z();

		shared_ptr<Texture> texture = ray.intersection.texture;

		if (texture && (texture->decal_mode == replace_all))
		{
			Color color_vec = texture->calculate_texture_color(intersection.intersection_point, ray.intersection.texture_vector);

            pixel_color.red = color_vec.red;
            pixel_color.green = color_vec.green;
            pixel_color.blue = color_vec.blue;
            
            return pixel_color;
		}

		calculate_degamma(material, camera.tonemap);
        if (!ray.inside_obj)
        {
            shared_ptr<Light> current_light;
            Point light_point;
            // iterates through all the area light sources
            for (int i = 0; i < all_lights.size(); i++)
            {
                current_light = all_lights[i];

                Vector3 direction = current_light->calculate_incoming_vector(ray.intersection);
                 bool is_shadow = check_light_shadow(ray,direction, current_light,light_point);
                                // calculates and adds shadings to the pixel color unless the ray is
                // shadow ray
                if (is_shadow == false)
                {       
	                Vector3 color = Vector3();         
					if (material.brdf_index > -1){
						// BRDF exists
						shared_ptr<BRDF> brdf = brdfs[material.brdf_index];

        				color = brdf->calculate_BRDF_component(ray, material, current_light);
					}
					else
                    {
                        Vector3 irradiance = current_light->calculate_irradiance(ray.intersection);
                        color = calculate_light_shadings(camera, material,irradiance, direction,
                                                            ray.intersection.intersection_point, ray.intersection.normal,ray.intersection.texture_vector, texture);
                    }                        
                    pixel_color.red += color.get_x();
                    pixel_color.green += color.get_y();
                    pixel_color.blue += color.get_z();
                }
            }
        }

        if (recursion_depth < max_recursion_depth)
        {
            if (material.material_type == MIRROR)
            {
                Color reflection;
                reflection.red = 0;
                reflection.green = 0;
                reflection.blue = 0;

                float wi = (-2) * ray.direction.dot_product(ray.intersection.normal);
                Vector3 normal_wi;
                normal_wi.set_x(ray.intersection.normal.get_x() * wi + ray.direction.get_x());
                normal_wi.set_y(ray.intersection.normal.get_y() * wi + ray.direction.get_y());
                normal_wi.set_z(ray.intersection.normal.get_z() * wi + ray.direction.get_z());

                normal_wi.normalize();

                Vector3 wi_epsilon;
                wi_epsilon.set_x(normal_wi.get_x() * this->shadow_ray_epsilon);
                wi_epsilon.set_y(normal_wi.get_y() * this->shadow_ray_epsilon);
                wi_epsilon.set_z(normal_wi.get_z() * this->shadow_ray_epsilon);

                if (material.roughness != 0)
                    normal_wi = calculate_roughness(normal_wi,material);

                Ray reflection_ray = Ray();
                reflection_ray = ray;
                reflection_ray.origin = ray.intersection.intersection_point + wi_epsilon;
                reflection_ray.direction = normal_wi;

                trace_ray(reflection_ray);

                //if (!(reflection_ray.intersection.p_shape == ray.intersection.p_shape))
                {
                    reflection = find_color(camera, reflection_ray, recursion_depth + 1, i, j);
                }

                pixel_color.red += reflection.red * material.mirror.get_x();
                pixel_color.blue += reflection.blue * material.mirror.get_y();
                pixel_color.green += reflection.green * material.mirror.get_z();
            }

            if (material.material_type == CONDUCTOR)
            {
                Vector3 reflected_direction =
                    ray.direction - ray.intersection.normal * (2 * ray.intersection.normal.dot_product(ray.direction));

                if (material.roughness != 0)
                    reflected_direction = calculate_roughness(reflected_direction,material);

                Ray reflection_ray = Ray();
                reflection_ray.origin = ray.intersection.intersection_point;
                reflection_ray.direction = reflected_direction;
                float reflection_ratio =
                    reflection_ratio_conductor(ray, material.refraction_index, material.absorption_index);

                Color reflection_color = find_color(camera, reflection_ray, (recursion_depth + 1), i, j);

                pixel_color.red += (material.mirror.get_x() * reflection_ratio * reflection_color.red);
                pixel_color.green += (material.mirror.get_y() * reflection_ratio * reflection_color.green);
                pixel_color.blue += (material.mirror.get_z() * reflection_ratio * reflection_color.blue);
            }

            if (material.material_type == DIELECTRIC)
            {

                Vector3 transmission_direction = Vector3(), transparency_factor = Vector3(1);
                Vector3 incident_direction = (ray.origin - ray.intersection.intersection_point).unit_vector();
                Vector3 reflection_direction =  (2 * (ray.intersection.normal * (incident_direction.dot_product(ray.intersection.normal))) -incident_direction).unit_vector();

                // cosine of the angle between ray direction and normal
                float cosine_angle = -1 * ray.direction.dot_product(ray.intersection.normal);
                bool does_enter = (cosine_angle > 0);
                float refraction_index1, refraction_index2;
                float reflection_ratio, transmission_ratio;

                refraction_index1 = 1;
                refraction_index2 = material.refraction_index;
                // refract the ray as it enters or exits the object
                if (does_enter) // ray enters
                {
                    transmission_direction =
                        ray_refraction(ray.direction, ray.intersection.normal, refraction_index1, refraction_index2);
                    reflection_ratio = reflection_ratio_dielectric(cosine_angle, refraction_index1, refraction_index2);
                    transmission_ratio = 1 - reflection_ratio;
                }
                else // ray exits
                {
                    cosine_angle = abs(cosine_angle);
                    transmission_direction = ray_refraction(ray.direction, -1 * ray.intersection.normal,
                                                            refraction_index2, refraction_index1);
                    reflection_ratio = reflection_ratio_dielectric(cosine_angle, refraction_index2, refraction_index1);
                    transmission_ratio = 1 - reflection_ratio;
                }

                Color additional_color;
                if (reflection_ratio != 1)
                {
                    transparency_factor.set_x(expf(-1 * material.absorption.get_x() * ray.intersection.distance));
                    transparency_factor.set_y(expf(-1 * material.absorption.get_y() * ray.intersection.distance));
                    transparency_factor.set_z(expf(-1 * material.absorption.get_z() * ray.intersection.distance));

                    Ray refraction_ray = Ray();
                    refraction_ray.origin =
                        ray.intersection.intersection_point + shadow_ray_epsilon * transmission_direction;
                    refraction_ray.direction = transmission_direction;
                    refraction_ray.inside_obj = does_enter;

                    Color refractionColor = find_color(camera, refraction_ray, recursion_depth + 1, i, j);

                    Ray reflection_ray = Ray();
                    reflection_ray.origin =
                        ray.intersection.intersection_point + shadow_ray_epsilon * reflection_direction;
                    reflection_ray.direction = reflection_direction;
                    reflection_ray.inside_obj = !does_enter;

                    Color reflection_color = find_color(camera, reflection_ray, recursion_depth + 1, i, j);

                    additional_color.red = transparency_factor.get_x() * (reflection_ratio * reflection_color.red) +
                                           (refractionColor.red * transmission_ratio);
                    additional_color.green = transparency_factor.get_x() * (reflection_ratio * reflection_color.green) +
                                             (refractionColor.green * transmission_ratio);
                    additional_color.blue = transparency_factor.get_x() * (reflection_ratio * reflection_color.blue) +
                                            (refractionColor.blue * transmission_ratio);
                }
                else
                {
                    Ray reflection_ray = Ray();
                    reflection_ray.origin =
                        ray.intersection.intersection_point + shadow_ray_epsilon * reflection_direction;
                    reflection_ray.direction = reflection_direction;
                    reflection_ray.inside_obj = true;

                    Color reflection_color = find_color(camera, reflection_ray, recursion_depth + 1, i, j);

                    // no refraction occurs, object is fully opaque
                    additional_color.red = transparency_factor.get_x() * reflection_color.red;
                    additional_color.green = transparency_factor.get_x() * reflection_color.green;
                    additional_color.blue = transparency_factor.get_x() * reflection_color.blue;
                }

                pixel_color.red += additional_color.red;
                pixel_color.green += additional_color.green;
                pixel_color.blue += additional_color.blue;
            }
        }
    }
    return pixel_color;
}

// returns reflection ratio
float Scene::reflection_ratio_dielectric(float cosine_angle, float refraction_index1, float refraction_index2)
{
    // Snell's law
    float sin_angle = sqrt(max(static_cast<double>(0), 1 - pow(cosine_angle, 2)));
    float sin_angle2 = (refraction_index1 / refraction_index2) * sin_angle;

    float ratio;

    if (sin_angle2 >= 1) // no refraction
        ratio = 1;
    else
    {
        float cosine_angle2 = sqrt(max(static_cast<double>(0), (1 - pow(sin_angle2, 2))));

        // Fresnel reflection rules
        float reflectance_s = (refraction_index2 * cosine_angle - refraction_index1 * cosine_angle2) /
                              (refraction_index2 * cosine_angle + refraction_index1 * cosine_angle2);
        float reflectance_p = (refraction_index1 * cosine_angle - refraction_index2 * cosine_angle2) /
                              (refraction_index1 * cosine_angle + refraction_index2 * cosine_angle2);

        ratio = (reflectance_s * reflectance_s + reflectance_p * reflectance_p) / 2;
    }
    return ratio;
}

float Scene::reflection_ratio_conductor(Ray &ray, float refractive_index, float absorption_index)
{
    // calculate the cosine of the angle between the surface normal and the incident direction
    float cosine_angle = (-1) * ray.intersection.normal.dot_product(ray.direction);
    float cosine_angle_sqr = pow(cosine_angle, 2);

    float sum_squares = pow(refractive_index, 2) + pow(absorption_index, 2);

    float two_n_cosine_angle = 2 * refractive_index * cosine_angle;

    // reflectance for s- and p-polarized light
    float reflectance_s =
        (sum_squares - two_n_cosine_angle + cosine_angle_sqr) / (sum_squares + cosine_angle_sqr + two_n_cosine_angle);
    float reflectance_p = (sum_squares * cosine_angle_sqr - two_n_cosine_angle + 1) /
                          (sum_squares * cosine_angle_sqr + two_n_cosine_angle + 1);

    float ratio = (reflectance_s + reflectance_p) / 2;
    return ratio;
}

/**
 * Traces the intersection point of ray to find shadow rays.
 *
 * @param ray contains the information about ray and its intersection
 * @param light light source
 */
bool Scene::check_light_shadow(Ray &ray,Vector3 incoming_dir_vector, shared_ptr<Light> a_light,Vector3 &a_light_point)
{
    // creates the shadow ray
    Ray shadow_ray = Ray();
    shadow_ray.time = ray.time;
    Vector3 point;

    shadow_ray.origin = ray.intersection.intersection_point + shadow_ray_epsilon * ray.intersection.normal;
    shadow_ray.direction = incoming_dir_vector;
    shadow_ray.shadow_ray = true;
    bool shadow = false;
    if (BVH_ON)
    {
        Intersection intersection = p_bounding_volume->intersect(shadow_ray);
        shadow_ray.intersection = intersection;
		float light_distance = a_light->calculate_distance(ray.intersection);

        if (intersection.does_hit == true && intersection.light_shape != a_light.get() && intersection.distance < light_distance - test_epsilon &&
            intersection.distance > 0)
        {

            shadow = true;
        }

    }
    else
    {
        float light_distance = (point - shadow_ray.origin ).length();
        // checks if the shadow ray hits any spheres
        for (int i = 0; i < spheres.size(); i++)
        {
            Intersection intersection = Intersection();
            intersection = spheres[i].intersect(shadow_ray);
            if (intersection.does_hit && intersection.distance > 0 && intersection.distance < light_distance - test_epsilon)
            {
                shadow = true;
                return shadow;
            }
        }
        // checks if the shadow ray hits any triangles
        for (int i = 0; i < triangles.size(); i++)
        {
            Intersection intersection = Intersection();
            intersection = triangles[i].intersect(shadow_ray);
            if (intersection.does_hit && intersection.distance > 0 && intersection.distance < light_distance - test_epsilon)
            {
                shadow = true;
                return shadow;
            }
        }

        // checks if the shadow ray hits any meshes
        for (int i = 0; i < meshes.size(); i++)
        {
            Intersection intersection = Intersection();
            vector<shared_ptr<Shape>> triangles = meshes[i].triangles;
            for (int i = 0; i < triangles.size(); i++)
            {
                Intersection intersection = Intersection();
                intersection = triangles[i]->intersect(shadow_ray);
                if (intersection.does_hit && intersection.distance > 0 &&
                    intersection.distance < light_distance - test_epsilon)
                {
                    shadow = true;
                    return shadow;
                }
            }
        }
    }

    return shadow;
}

/**
 * Traces the ray and checks if there is an intersection.
 *
 * @param ray contains the information about ray and its intersection
 */
void Scene::trace_ray(Ray &ray)
{
    // checks if there is an intersection with triangles
    for (int i = 0; i < triangles.size(); i++)
    {
        Intersection intersection = Intersection();
        intersection = triangles[i].intersect(ray);
        if (intersection.does_hit && intersection.distance > 0 &&
            intersection.distance < ray.intersection.distance - test_epsilon)
        {
            ray.intersection = intersection;
        }
    }
    // checks if there is an intersection with meshes
    for (int i = 0; i < meshes.size(); i++)
    {
        Intersection intersection = Intersection();
        intersection = meshes[i].intersect(ray);
        if (intersection.does_hit && intersection.distance > 0 &&
            intersection.distance < ray.intersection.distance - test_epsilon)
        {
            ray.intersection = intersection;
        }
    }

    // checks if there is an intersection with spheres
    for (int i = 0; i < spheres.size(); i++)
    {
        Intersection intersection = Intersection();
        intersection = spheres[i].intersect(ray);
        if (intersection.does_hit && intersection.distance > 0 &&
            intersection.distance < ray.intersection.distance - test_epsilon)
        {
            ray.intersection = intersection;
        }
    }
}

// Jittered Multisampling
void Scene::ray_sampling(Camera& camera,bool is_dof, int i, int j,vector<shared_ptr<Ray>> &raysPerPixel)
{
	int number_of_pixels = sqrt(camera.number_of_samples);
    float dof_x_random = 1,dof_y_random = 1;

	for (int m = 0; m < number_of_pixels; m++)
	{
		for (int n = 0; n < number_of_pixels; n++)
		{
			float randx = gNURandomDistribution(gRandomGenerator) + 0.5;
			float randy = gNURandomDistribution(gRandomGenerator) + 0.5;

			float dx = (m + randx) / number_of_pixels;
			float dy = (n + randy) / number_of_pixels;

            if(is_dof)
            {
                dof_x_random = gNURandomDistribution(gRandomGenerator);
			    dof_y_random = gNURandomDistribution(gRandomGenerator);
            }

			float time = gNURandomDistribution(gRandomGenerator) + 0.5;

            shared_ptr<Ray> ray = make_shared<Ray>();
			ray = camera.generate_primary_ray(i, j, is_dof,  time, dx, dy, dof_x_random, dof_y_random);
			raysPerPixel.push_back(ray);
		}
	}
}


/**
 * Iterates through all the pixels one by one and finds the color.
 *
 * @param start_row
 * @param end_row
 * @param image_width
 * @param camera
 * @param image
 */
void Scene::render_chunk(int start_row, int end_row, int start_col, int end_col, int image_width, Camera &camera)
{
    float a_time = gNURandomDistribution(gRandomGenerator);

    Color pixel_color = Color();
    int number_of_samples = camera.number_of_samples;
    int color_index;

   int pixel_index = 0;
    if (number_of_samples != 1) // multisampling method
	{
        #pragma omp parallel for
        for (int i = start_row; i < end_row; i++)
        {
            #pragma omp parallel for
            for (int j = start_col; j < end_col; j++)
            {

                pixel_index = i * image_width + j ;
                vector<shared_ptr<Ray>> raysPerPixel;

                if (camera.aperture_size != 0)
                    ray_sampling(camera,true, i, j,raysPerPixel);
                else
                    ray_sampling(camera,false, i, j,raysPerPixel);
                
                for (int r = 0; r < raysPerPixel.size(); r++)
                {
                    Ray ray = *raysPerPixel[r];
                    pixel_color += find_color(camera, *raysPerPixel[r], 0, i, j);
                }

                pixel_color.div_num_samples(number_of_samples);

                pixelColors[pixel_index] = pixel_color; 
            }
        }
    }
    else // naive method
    {
        for (int i = start_row; i < end_row; i++)
        {
	        int pixel_index = (i) * image_width + start_col;
            for (int j = start_col; j < end_col; j++)
            {
                shared_ptr<Ray> ray = camera.generate_primary_ray(i, j, false, a_time);

                pixel_color = find_color(camera, *ray, 0, i, j);

                pixelColors[pixel_index++] = pixel_color; 
            }
        }
    }
}
//Converts a rgb float Vector color to Uint32 rgba 8 bit color
Uint32 Scene::vector3_to_uint32(Color color)
{
	int r = int( color.red);
    if (r > 255)
        r = 255;
    else if(r < 0)
        r = 0;
	int g = int( color.green);
    if (g > 255)
        g = 255;
    else if(g < 0)
        g = 0;
    
	int b = int( color.blue);
    if (b > 255)
        b = 255;
    else if(b < 0)
        b = 0;
    
	int a = 255;
    
	auto result = r << 24 | g << 16 | b << 8 | a;
	return result;
}

void Scene::convertToUint32() 
{ 
    for (int i = 0; i < pixelColors.size();i++) 
    { 
        Color color = pixelColors[i]; 
        
        int r=color.red , g=color.green, b=color.blue, a=255;
	    auto result = vector3_to_uint32(color);
        pixels[i] = result;

        //int r=120 , g=120, b=120, a=120;
        //pixels[i] = r << 24 | g << 16 | b << 8 | a;
    } 
}

void Scene::SDL_thread_start(int image_width, int image_height, std::string image_name) {
    int success = SDL_image_create( image_width, image_height, image_name);
    while (program_running) 
    {     
        SDL_Event event;
        SDL_PollEvent(&event);

        Sleep(25);
        SDL_image_renderer(); 
    }
        SDL_image_finalize();
}
int Scene::SDL_image_create(int image_width, int image_height, std::string image_name) {
    sdl_image_width=image_width;
    sdl_image_height=image_height;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 
            return -1;

    window = SDL_CreateWindow(image_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                image_width, image_height,SDL_WINDOW_SHOWN);
    if (!window) 
        return -1;

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) 
        return -1;
    
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, sdl_image_width,
	                                         sdl_image_height);


	//RGBA 8 bit color for displaying
	pixels = new Uint32[sdl_image_width * sdl_image_height];
	memset(pixels, 0, sizeof(Uint32) * sdl_image_width * sdl_image_height);
	SDL_SetRenderDrawColor(renderer, 50, 100, 50, 255);
	SDL_RenderClear(renderer);

    SDL_UpdateTexture(texture, NULL, pixels, sizeof(Uint32) * sdl_image_width);
    SDL_RenderCopy(renderer, texture,NULL,NULL);
    SDL_RenderPresent(renderer);

    return 0;
}
void Scene::SDL_image_renderer() {
    convertToUint32();//Output color is corrected

    SDL_UpdateTexture(texture, NULL, pixels, sizeof(Uint32) * sdl_image_width);	//Ouput to screen
    SDL_RenderCopy(renderer, texture,NULL,NULL);
    SDL_RenderPresent(renderer);
}

void Scene::SDL_image_finalize(){
    SDL_Delay(2000);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

Color Scene::calculate_bg_pixel_color( Ray& ray, int i, int j) const
{
    Color pixel_color = Color();

	if (bg_texture)
    {    
        i %= bg_texture->img_width;
        j %= bg_texture->img_height;

        Vector2 uv = Vector2();
        uv.x = i / (float)bg_texture->img_width;
        uv.y = j / (float)bg_texture->img_height;
        Vector3 temp = Vector3();
        Color texture_color = bg_texture->calculate_texture_color(temp,uv);
        pixel_color.red = texture_color.red;
        pixel_color.green = texture_color.green;
        pixel_color.blue = texture_color.blue; 
    }   
    else if (spherical_dir_light)
		pixel_color = spherical_dir_light->get_texture_color(ray.direction);
    else 
    {
        pixel_color.red = background_color.x;
        pixel_color.green = background_color.y;
        pixel_color.blue = background_color.z;   
    }

	return pixel_color;
}