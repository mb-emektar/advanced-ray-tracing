
del:: raytracer.exe

cmake -G "MinGW Makefiles"

mingw32-make.exe

set outputDir=output_images

if not exist %outputDir% mkdir %outputDir%
if not exist %outputDir%\hw1_outputs mkdir %outputDir%\hw1_outputs
if not exist %outputDir%\hw2_outputs mkdir %outputDir%\hw2_outputs
if not exist %outputDir%\hw3_outputs mkdir %outputDir%\hw3_outputs
if not exist %outputDir%\hw4_outputs mkdir %outputDir%\hw4_outputs
if not exist %outputDir%\hw5_outputs mkdir %outputDir%\hw5_outputs
if not exist %outputDir%\hw6_outputs mkdir %outputDir%\hw6_outputs
if not exist %outputDir%\project_outputs mkdir %outputDir%\project_outputs

del %outputDir%\hw1_outputs\log.txt
del %outputDir%\hw2_outputs\log.txt
del %outputDir%\hw3_outputs\log.txt
del %outputDir%\hw4_outputs\log.txt
del %outputDir%\hw5_outputs\log.txt
del %outputDir%\hw6_outputs\log.txt

GOTO :project
:: :::::::::::::::::::::::: hw 1 inputs ::::::::::::::::::::::::

:::: raytracer.exe ../inputs/hw1_inputs/bunny.xml >> %outputDir%\hw1_outputs\log.txt
:::: move bunny.png %outputDir%\hw1_outputs\bunny.png

:::: raytracer.exe ../inputs/hw1_inputs/chinese_dragon.xml >> %outputDir%\hw1_outputs\log.txt
:::: move chinese_dragon.png %outputDir%\hw1_outputs\chinese_dragon.png

:::: raytracer.exe ../inputs/hw1_inputs/cornellbox.xml >> %outputDir%\hw1_outputs\log.txt
:::: move cornellbox.png %outputDir%\hw1_outputs\cornellbox.png

:::: raytracer.exe ../inputs/hw1_inputs/cornellbox_recursive.xml >> %outputDir%\hw1_outputs\log.txt
:::: move cornellbox_recursive.png %outputDir%\hw1_outputs\cornellbox_recursive.png

:::: raytracer.exe ../inputs/hw1_inputs/lobster.xml >> %outputDir%\hw1_outputs\log.txt
:::: move lobster.png %outputDir%\hw1_outputs\lobster.png

:::: raytracer.exe ../inputs/hw1_inputs/other_dragon.xml >> %outputDir%\hw1_outputs\log.txt
:::: move other_dragon.png %outputDir%\hw1_outputs\other_dragon.png

:::: raytracer.exe ../inputs/hw1_inputs/scienceTree.xml >> %outputDir%\hw1_outputs\log.txt
:::: move scienceTree.png %outputDir%\hw1_outputs\scienceTree.png

:::: raytracer.exe ../inputs/hw1_inputs/scienceTree_glass.xml >> %outputDir%\hw1_outputs\log.txt
:::: move scienceTree_glass.png %outputDir%\hw1_outputs\scienceTree_glass.png

:::: raytracer.exe ../inputs/hw1_inputs/simple.xml >> %outputDir%\hw1_outputs\log.txt
:::: move simple.png %outputDir%\hw1_outputs\simple.png

:::: raytracer.exe ../inputs/hw1_inputs/spheres.xml >> %outputDir%\hw1_outputs\log.txt
:::: move spheres.png %outputDir%\hw1_outputs\spheres.png

:::: raytracer.exe ../inputs/hw1_inputs/spheres_mirror.xml >> %outputDir%\hw1_outputs\log.txt
:::: move spheres_mirror.png %outputDir%\hw1_outputs\spheres_mirror.png

:::: raytracer.exe ../inputs/hw1_inputs/ton_Roosendaal_smooth.xml >> %outputDir%\hw1_outputs\log.txt
:::: move ton_Roosendaal_smooth.png %outputDir%\hw1_outputs\ton_Roosendaal_smooth.png

:::: raytracer.exe ../inputs/hw1_inputs/trex_smooth.xml >> %outputDir%\hw1_outputs\log.txt
:::: move trex_smooth.png %outputDir%\hw1_outputs\trex_smooth.png

:::: raytracer.exe ../inputs/hw1_inputs/two_spheres.xml >> %outputDir%\hw1_outputs\log.txt
:::: move two_spheres.png %outputDir%\hw1_outputs\two_spheres.png

:::: raytracer.exe ../inputs/hw1_inputs/berserker_smooth.xml >> %outputDir%\hw1_outputs\log.txt
:::: move berserker_smooth.png %outputDir%\hw1_outputs\berserker_smooth.png

:::: raytracer.exe ../inputs/hw1_inputs/car_smooth_fixed.xml >> %outputDir%\hw1_outputs\log.txt
:::: move Car_smooth.png %outputDir%\hw1_outputs\Car_smooth.png
:::: move Car_front_smooth.png %outputDir%\hw1_outputs\Car_front_smooth.png

:::: raytracer.exe ../inputs/hw1_inputs/low_poly_smooth.xml >> %outputDir%\hw1_outputs\log.txt
:::: move low_poly_scene_smooth.png %outputDir%\hw1_outputs\low_poly_scene_smooth.png

:::: raytracer.exe ../inputs/hw1_inputs/tower_smooth.xml >> %outputDir%\hw1_outputs\log.txt
:::: move tower_smooth.png %outputDir%\hw1_outputs\tower_smooth.png

:::: raytracer.exe ../inputs/hw1_inputs/windmill_smooth.xml >> %outputDir%\hw1_outputs\log.txt
:::: move windmill_smooth.png %outputDir%\hw1_outputs\windmill_smooth.png

:: :::::::::::::::::::::::: hw 2 inputs ::::::::::::::::::::::::

:::: raytracer.exe ../inputs/hw2_inputs/two_berserkers.xml >> %outputDir%\hw2_outputs\log.txt
:::: move two_berserkers.png %outputDir%\hw2_outputs\two_berserkers.png

:::: raytracer.exe ../inputs/hw2_inputs/grass_desert.xml >> %outputDir%\hw2_outputs\log.txt
:::: move grass_desert.png %outputDir%\hw2_outputs\grass_desert.png

:::: raytracer.exe ../inputs/hw2_inputs/dragon_metal.xml >> %outputDir%\hw2_outputs\log.txt
:::: move dragon_metal.png %outputDir%\hw2_outputs\dragon_metal.png

:::: raytracer.exe ../inputs/hw2_inputs/ellipsoids.xml >> %outputDir%\hw2_outputs\log.txt
:::: move ellipsoids.png %outputDir%\hw2_outputs\ellipsoids.png

:::: raytracer.exe ../inputs/hw2_inputs/marching_dragons.xml >> %outputDir%\hw2_outputs\log.txt
:::: move marching_dragons.png %outputDir%\hw2_outputs\marching_dragons.png

:::: raytracer.exe ../inputs/hw2_inputs/metal_glass_plates.xml >> %outputDir%\hw2_outputs\log.txt
:::: move metal_glass_plates.png %outputDir%\hw2_outputs\metal_glass_plates.png

:::: raytracer.exe ../inputs/hw2_inputs/simple_transform.xml >> %outputDir%\hw2_outputs\log.txt
:::: move simple_transform.png %outputDir%\hw2_outputs\simple_transform.png

:::: raytracer.exe ../inputs/hw2_inputs/spheres.xml >> %outputDir%\hw2_outputs\log.txt
:::: move spheres.png %outputDir%\hw2_outputs\spheres.png

:: :::::::::::::::::::::::: hw 3 inputs ::::::::::::::::::::::::

:::: raytracer.exe ../inputs/hw3_inputs/cornellbox_area.xml >> %outputDir%\hw3_outputs\log.txt
:::: move cornellbox_area.png %outputDir%\hw3_outputs\cornellbox_area.png

:::: raytracer.exe ../inputs/hw3_inputs/cornellbox_boxes_dynamic.xml >> %outputDir%\hw3_outputs\log.txt
:::: move cornellbox_boxes_dynamic.png %outputDir%\hw3_outputs\cornellbox_boxes_dynamic.png

:::: raytracer.exe ../inputs/hw3_inputs/cornellbox_brushed_metal.xml >> %outputDir%\hw3_outputs\log.txt
:::: move cornellbox_brushed_metal.png %outputDir%\hw3_outputs\cornellbox_brushed_metal.png

:::: raytracer.exe ../inputs/hw3_inputs/focusing_dragons.xml >> %outputDir%\hw3_outputs\log.txt
:::: move focusing_dragons.png %outputDir%\hw3_outputs\focusing_dragons.png

:::: raytracer.exe ../inputs/hw3_inputs/dragon_dynamic.xml >> %outputDir%\hw3_outputs\log.txt
:::: move dragon_dynamic.png %outputDir%\hw3_outputs\dragon_dynamic.png

 ::::raytracer.exe ../inputs/hw3_inputs/metal_glass_plates.xml >> %outputDir%\hw3_outputs\log.txt
 ::::move metal_glass_plates.png %outputDir%\hw3_outputs\metal_glass_plates.png

:::: raytracer.exe ../inputs/hw3_inputs/spheres_dof.xml >> %outputDir%\hw3_outputs\log.txt
:::: move spheres_dof.png %outputDir%\hw3_outputs\spheres_dof.png

:: :::::::::::::::::::::::: hw 4 inputs ::::::::::::::::::::::::

:::: raytracer.exe ../inputs/hw4_inputs/brickwall_with_normalmap.xml >> %outputDir%\hw4_outputs\log.txt
:::: move brickwall_with_normalmap.png %outputDir%\hw4_outputs\brickwall_with_normalmap.png

:::: raytracer.exe ../inputs/hw4_inputs/bump_mapping_transformed.xml >> %outputDir%\hw4_outputs\log.txt
:::: move bump_mapping_transformed.png %outputDir%\hw4_outputs\bump_mapping_transformed.png

:::: raytracer.exe ../inputs/hw4_inputs/cube_cushion.xml >> %outputDir%\hw4_outputs\log.txt
:::: move cube_cushion.png %outputDir%\hw4_outputs\cube_cushion.png

 ::::raytracer.exe ../inputs/hw4_inputs/cube_perlin.xml >> %outputDir%\hw4_outputs\log.txt
:::: move cube_perlin.png %outputDir%\hw4_outputs\cube_perlin.png

:::: raytracer.exe ../inputs/hw4_inputs/cube_perlin_bump.xml >> %outputDir%\hw4_outputs\log.txt
:::: move cube_perlin_bump.png %outputDir%\hw4_outputs\cube_perlin_bump.png

:::: raytracer.exe ../inputs/hw4_inputs/cube_wall.xml >> %outputDir%\hw4_outputs\log.txt
:::: move cube_wall.png %outputDir%\hw4_outputs\cube_wall.png

 ::::raytracer.exe ../inputs/hw4_inputs/cube_wall_normal.xml >> %outputDir%\hw4_outputs\log.txt
:::: move cube_wall_normal.png %outputDir%\hw4_outputs\cube_wall_normal.png

:::: raytracer.exe ../inputs/hw4_inputs/cube_waves.xml >> %outputDir%\hw4_outputs\log.txt
:::: move cube_waves.png %outputDir%\hw4_outputs\cube_waves.png

:::: raytracer.exe ../inputs/hw4_inputs/ellipsoids_texture.xml >> %outputDir%\hw4_outputs\log.txt
:::: move ellipsoids_texture.png %outputDir%\hw4_outputs\ellipsoids_texture.png

:::: raytracer.exe ../inputs/hw4_inputs/galactica_dynamic.xml >> %outputDir%\hw4_outputs\log.txt
:::: move galactica_dynamic.png %outputDir%\hw4_outputs\galactica_dynamic.png

:::: raytracer.exe ../inputs/hw4_inputs/galactica_static.xml >> %outputDir%\hw4_outputs\log.txt
:::: move galactica_static.png %outputDir%\hw4_outputs\galactica_static.png

:::: raytracer.exe ../inputs/hw4_inputs/killeroo_bump_walls.xml >> %outputDir%\hw4_outputs\log.txt
:::: move killeroo_bump_walls.png %outputDir%\hw4_outputs\killeroo_bump_walls.png

:::: raytracer.exe ../inputs/hw4_inputs/plane_bilinear.xml >> %outputDir%\hw4_outputs\log.txt
:::: move plane_bilinear.png %outputDir%\hw4_outputs\plane_bilinear.png

:::: raytracer.exe ../inputs/hw4_inputs/plane_nearest.xml >> %outputDir%\hw4_outputs\log.txt
:::: move plane_nearest.png %outputDir%\hw4_outputs\plane_nearest.png

:::: raytracer.exe ../inputs/hw4_inputs/plane_trilinear.xml >> %outputDir%\hw4_outputs\log.txt
:::: move plane_trilinear.png %outputDir%\hw4_outputs\plane_trilinear.png

:::: raytracer.exe ../inputs/hw4_inputs/sphere_nearest_bilinear.xml >> %outputDir%\hw4_outputs\log.txt
:::: move sphere_nearest_bilinear.png %outputDir%\hw4_outputs\sphere_nearest_bilinear.png

:::: raytracer.exe ../inputs/hw4_inputs/sphere_nearest_trilinear.xml >> %outputDir%\hw4_outputs\log.txt
 ::move sphere_nearest_trilinear.png %outputDir%\hw4_outputs\sphere_nearest_trilinear.png

:::: raytracer.exe ../inputs/hw4_inputs/sphere_nobump_bump.xml >> %outputDir%\hw4_outputs\log.txt
:::: move sphere_nobump_bump.png %outputDir%\hw4_outputs\sphere_nobump_bump.png

:::: raytracer.exe ../inputs/hw4_inputs/sphere_nobump_justbump.xml >> %outputDir%\hw4_outputs\log.txt
:::: move sphere_nobump_justbump.png %outputDir%\hw4_outputs\sphere_nobump_justbump.png

:::: raytracer.exe ../inputs/hw4_inputs/sphere_normal.xml >> %outputDir%\hw4_outputs\log.txt
:::: move sphere_normal.png %outputDir%\hw4_outputs\sphere_normal.png

:::: raytracer.exe ../inputs/hw4_inputs/sphere_perlin.xml >> %outputDir%\hw4_outputs\log.txt
:::: move sphere_perlin.png %outputDir%\hw4_outputs\sphere_perlin.png

 ::raytracer.exe ../inputs/hw4_inputs/sphere_perlin_bump.xml >> %outputDir%\hw4_outputs\log.txt
 ::move sphere_perlin_bump.png %outputDir%\hw4_outputs\sphere_perlin_bump.png

:::: raytracer.exe ../inputs/hw4_inputs/sphere_perlin_scale.xml >> %outputDir%\hw4_outputs\log.txt
:::: move sphere_perlin_scale.png %outputDir%\hw4_outputs\sphere_perlin_scale.png

:::: raytracer.exe ../inputs/hw4_inputs/wood_box.xml >> %outputDir%\hw4_outputs\log.txt
:::: move wood_box.png %outputDir%\hw4_outputs\wood_box.png

:::: raytracer.exe ../inputs/hw4_inputs/wood_box_all.xml >> %outputDir%\hw4_outputs\log.txt
:::: move wood_box_all.png %outputDir%\hw4_outputs\wood_box_all.png

:::: raytracer.exe ../inputs/hw4_inputs/wood_box_no_specular.xml >> %outputDir%\hw4_outputs\log.txt
:::: move wood_box_no_specular.png %outputDir%\hw4_outputs\wood_box_no_specular.png

:: raytracer.exe ../inputs/hw4_inputs/dragon/dragon_new_ply.xml >> %outputDir%\hw4_outputs\log.txt
:: move dragon_new.png %outputDir%\hw4_outputs\dragon_new.png

:::: raytracer.exe ../inputs/hw4_inputs/mytap/mytap_final.xml >> %outputDir%\hw4_outputs\log.txt
:::: move mytap_final.png %outputDir%\hw4_outputs\mytap_final.png

:::: raytracer.exe ../inputs/hw4_inputs/veach_ajar/scene.xml >> %outputDir%\hw4_outputs\log.txt
:::: move VeachAjar.png %outputDir%\hw4_outputs\VeachAjar.png

:: :::::::::::::::::::::::: hw 5 inputs ::::::::::::::::::::::::

:: raytracer.exe ../inputs/hw5_inputs/cube_directional.xml >> %outputDir%\hw5_outputs\log.txt
:: move cube_directional.png %outputDir%\hw5_outputs\cube_directional.png

:: raytracer.exe ../inputs/hw5_inputs/cube_point.xml >> %outputDir%\hw5_outputs\log.txt
:: move cube_point.png %outputDir%\hw5_outputs\cube_point.png

:: raytracer.exe ../inputs/hw5_inputs/cube_point_hdr.xml >> %outputDir%\hw5_outputs\log.txt
:: move cube_point_hdr.exr %outputDir%\hw5_outputs\cube_point_hdr.exr
:: move cube_point_hdr.png %outputDir%\hw5_outputs\cube_point_hdr.png

:: raytracer.exe ../inputs/hw5_inputs/dragon_spot_light_msaa.xml >> %outputDir%\hw5_outputs\log.txt
:: move dragon_spot_light_msaa.png %outputDir%\hw5_outputs\dragon_spot_light_msaa.png

:: raytracer.exe ../inputs/hw5_inputs/empty_environment_latlong.xml >> %outputDir%\hw5_outputs\log.txt
:: move empty_environment_latlong.exr %outputDir%\hw5_outputs\empty_environment_latlong.exr
:: move empty_environment_latlong.png %outputDir%\hw5_outputs\empty_environment_latlong.png

:: raytracer.exe ../inputs/hw5_inputs/empty_environment_light_probe.xml >> %outputDir%\hw5_outputs\log.txt
:: move empty_environment_light_probe.exr %outputDir%\hw5_outputs\empty_environment_light_probe.exr
:: move empty_environment_light_probe.png %outputDir%\hw5_outputs\empty_environment_light_probe.png

:: raytracer.exe ../inputs/hw5_inputs/glass_sphere_env.xml >> %outputDir%\hw5_outputs\log.txt
:: move glass_sphere_env.exr %outputDir%\hw5_outputs\glass_sphere_env.exr
:: move glass_sphere_env.png %outputDir%\hw5_outputs\glass_sphere_env.png

:: raytracer.exe ../inputs/hw5_inputs/head_env_light.xml >> %outputDir%\hw5_outputs\log.txt
:: move head_env_light.exr %outputDir%\hw5_outputs\head_env_light.exr
:: move head_env_light.png %outputDir%\hw5_outputs\head_env_light.png

:: raytracer.exe ../inputs/hw5_inputs/mirror_sphere_env.xml >> %outputDir%\hw5_outputs\log.txt
:: move mirror_sphere_env.exr %outputDir%\hw5_outputs\mirror_sphere_env.exr
:: move mirror_sphere_env.png %outputDir%\hw5_outputs\mirror_sphere_env.png

:: raytracer.exe ../inputs/hw5_inputs/sphere_env_light.xml >> %outputDir%\hw5_outputs\log.txt
:: move sphere_env_light.exr %outputDir%\hw5_outputs\sphere_env_light.exr
:: move sphere_env_light.png %outputDir%\hw5_outputs\sphere_env_light.png

:: raytracer.exe ../inputs/hw5_inputs/sphere_point_hdr_texture.xml >> %outputDir%\hw5_outputs\log.txt
:: move sphere_point_hdr_texture.exr %outputDir%\hw5_outputs\sphere_point_hdr_texture.exr
:: move sphere_point_hdr_texture.png %outputDir%\hw5_outputs\sphere_point_hdr_texture.png

:: raytracer.exe ../inputs/hw5_inputs/tt/scene_glacier.xml >> %outputDir%\hw5_outputs\log.txt
:: move audi-tt-glacier.exr %outputDir%\hw5_outputs\audi-tt-glacier.exr
:: move audi-tt-glacier.png %outputDir%\hw5_outputs\audi-tt-glacier.png

:: raytracer.exe ../inputs/hw5_inputs/tt/scene_pisa.xml >> %outputDir%\hw5_outputs\log.txt
:::: move audi-tt-pisa.exr %outputDir%\hw5_outputs\audi-tt-pisa.exr
:::: move audi-tt-pisa.png %outputDir%\hw5_outputs\audi-tt-pisa.png

:::: raytracer.exe ../inputs/hw5_inputs/dragon/dragon_new_ply_with_spot.xml >> %outputDir%\hw5_outputs\log.txt
:::: move dragon_new_with_spot.png %outputDir%\hw5_outputs\dragon_new_with_spot.png

:: :::::::::::::::::::::::: hw 6 inputs ::::::::::::::::::::::::

   raytracer.exe ../inputs/hw6_inputs/brdf/inputs/brdf_blinnphong_modified.xml >> %outputDir%\hw6_outputs\log.txt
   move brdf_blinnphong_modified.png %outputDir%\hw6_outputs\brdf_blinnphong_modified.png

   raytracer.exe ../inputs/hw6_inputs/brdf/inputs/brdf_blinnphong_modified_normalized.xml >> %outputDir%\hw6_outputs\log.txt
   move brdf_blinnphong_modified_normalized.png %outputDir%\hw6_outputs\brdf_blinnphong_modified_normalized.png

   raytracer.exe ../inputs/hw6_inputs/brdf/inputs/brdf_blinnphong_original.xml >> %outputDir%\hw6_outputs\log.txt
   move brdf_blinnphong_original.png %outputDir%\hw6_outputs\brdf_blinnphong_original.png

   raytracer.exe ../inputs/hw6_inputs/brdf/inputs/brdf_phong_modified.xml >> %outputDir%\hw6_outputs\log.txt
   move brdf_phong_modified.png %outputDir%\hw6_outputs\brdf_phong_modified.png

   raytracer.exe ../inputs/hw6_inputs/brdf/inputs/brdf_phong_modified_normalized.xml >> %outputDir%\hw6_outputs\log.txt
   move brdf_phong_modified_normalized.png %outputDir%\hw6_outputs\brdf_phong_modified_normalized.png

   raytracer.exe ../inputs/hw6_inputs/brdf/inputs/brdf_phong_original.xml >> %outputDir%\hw6_outputs\log.txt
   move brdf_phong_original.png %outputDir%\hw6_outputs\brdf_phong_original.png

   raytracer.exe ../inputs/hw6_inputs/brdf/inputs/brdf_torrancesparrow.xml >> %outputDir%\hw6_outputs\log.txt
   move brdf_torrancesparrow.png %outputDir%\hw6_outputs\brdf_torrancesparrow.png

  raytracer.exe ../inputs/hw6_inputs/brdf/inputs/killeroo_blinnphong.xml >> %outputDir%\hw6_outputs\log.txt
 move killeroo_blinnphong.png %outputDir%\hw6_outputs\killeroo_blinnphong.png
 move killeroo_blinnphong.exr %outputDir%\hw6_outputs\killeroo_blinnphong.exr
 move killeroo_blinnphong_closeup.png %outputDir%\hw6_outputs\killeroo_blinnphong_closeup.png
 move killeroo_blinnphong_closeup.exr %outputDir%\hw6_outputs\killeroo_blinnphong_closeup.exr

 raytracer.exe ../inputs/hw6_inputs/brdf/inputs/killeroo_torrancesparrow.xml >> %outputDir%\hw6_outputs\log.txt
 move killeroo_torrancesparrow.png %outputDir%\hw6_outputs\killeroo_torrancesparrow.png
 move killeroo_torrancesparrow.exr %outputDir%\hw6_outputs\killeroo_torrancesparrow.exr
 move killeroo_torrancesparrow_closeup.png %outputDir%\hw6_outputs\killeroo_torrancesparrow_closeup.png
 move killeroo_torrancesparrow_closeup.exr %outputDir%\hw6_outputs\killeroo_torrancesparrow_closeup.exr

::::::::::::::::::::::::

 raytracer.exe ../inputs/hw6_inputs/directLighting/inputs/cornellbox_jaroslav_diffuse.xml >> %outputDir%\hw6_outputs\log.txt
 move cornellbox_jaroslav_diffuse.png %outputDir%\hw6_outputs\cornellbox_jaroslav_diffuse.png
 move cornellbox_jaroslav_diffuse.exr %outputDir%\hw6_outputs\cornellbox_jaroslav_diffuse.exr

 raytracer.exe ../inputs/hw6_inputs/directLighting/inputs/cornellbox_jaroslav_diffuse_area.xml >> %outputDir%\hw6_outputs\log.txt
 move cornellbox_jaroslav_diffuse_area.png %outputDir%\hw6_outputs\cornellbox_jaroslav_diffuse_area.png
 move cornellbox_jaroslav_diffuse_area.exr %outputDir%\hw6_outputs\cornellbox_jaroslav_diffuse_area.exr

 raytracer.exe ../inputs/hw6_inputs/directLighting/inputs/cornellbox_jaroslav_glossy.xml >> %outputDir%\hw6_outputs\log.txt
 move cornellbox_jaroslav_glossy.png %outputDir%\hw6_outputs\cornellbox_jaroslav_glossy.png
 move cornellbox_jaroslav_glossy.exr %outputDir%\hw6_outputs\cornellbox_jaroslav_glossy.exr

 raytracer.exe ../inputs/hw6_inputs/directLighting/inputs/cornellbox_jaroslav_glossy_area.xml >> %outputDir%\hw6_outputs\log.txt
 move cornellbox_jaroslav_glossy_area.png %outputDir%\hw6_outputs\cornellbox_jaroslav_glossy_area.png
 move cornellbox_jaroslav_glossy_area.exr %outputDir%\hw6_outputs\cornellbox_jaroslav_glossy_area.exr

 raytracer.exe ../inputs/hw6_inputs/directLighting/inputs/cornellbox_jaroslav_glossy_area_ellipsoid.xml >> %outputDir%\hw6_outputs\log.txt
 move cornellbox_jaroslav_glossy_area_ellipsoid.png %outputDir%\hw6_outputs\cornellbox_jaroslav_glossy_area_ellipsoid.png
 move cornellbox_jaroslav_glossy_area_ellipsoid.exr %outputDir%\hw6_outputs\cornellbox_jaroslav_glossy_area_ellipsoid.exr

 raytracer.exe ../inputs/hw6_inputs/directLighting/inputs/cornellbox_jaroslav_glossy_area_small.xml >> %outputDir%\hw6_outputs\log.txt
 move cornellbox_jaroslav_glossy_area_small.png %outputDir%\hw6_outputs\cornellbox_jaroslav_glossy_area_small.png
 move cornellbox_jaroslav_glossy_area_small.exr %outputDir%\hw6_outputs\cornellbox_jaroslav_glossy_area_small.exr

 raytracer.exe ../inputs/hw6_inputs/directLighting/inputs/cornellbox_jaroslav_glossy_area_sphere.xml >> %outputDir%\hw6_outputs\log.txt
 move cornellbox_jaroslav_glossy_area_sphere.png %outputDir%\hw6_outputs\cornellbox_jaroslav_glossy_area_sphere.png
 move cornellbox_jaroslav_glossy_area_sphere.exr %outputDir%\hw6_outputs\cornellbox_jaroslav_glossy_area_sphere.exr
::::::::::::::::::::::::

 raytracer.exe ../inputs/hw6_inputs/pathTracing/inputs/cornellbox_jaroslav_path_diffuse.xml >> %outputDir%\hw6_outputs\log.txt
 move diffuse_test.png %outputDir%\hw6_outputs\diffuse_test.png
 move diffuse_test.exr %outputDir%\hw6_outputs\diffuse_test.exr

 raytracer.exe ../inputs/hw6_inputs/pathTracing/inputs/cornellbox_jaroslav_path_diffuse_gt.xml >> %outputDir%\hw6_outputs\log.txt
 move diffuse_cosine_importance_gt.png %outputDir%\hw6_outputs\diffuse_cosine_importance_gt.png
 move diffuse_cosine_importance_gt.exr %outputDir%\hw6_outputs\diffuse_cosine_importance_gt.exr

 raytracer.exe ../inputs/hw6_inputs/pathTracing/inputs/cornellbox_jaroslav_path_diffuse_russian.xml >> %outputDir%\hw6_outputs\log.txt
 move diffuse_russian.png %outputDir%\hw6_outputs\diffuse_russian.png
 move diffuse_russian.exr %outputDir%\hw6_outputs\diffuse_russian.exr
 move diffuse_splitting.png %outputDir%\hw6_outputs\diffuse_splitting.png
 move diffuse_splitting.exr %outputDir%\hw6_outputs\diffuse_splitting.exr

 raytracer.exe ../inputs/hw6_inputs/pathTracing/inputs/cornellbox_jaroslav_path_diffuse_splitting.xml >> %outputDir%\hw6_outputs\log.txt
 move diffuse_no_splitting.png %outputDir%\hw6_outputs\diffuse_no_splitting.png
 move diffuse_no_splitting.exr %outputDir%\hw6_outputs\diffuse_no_splitting.exr
 move diffuse.png %outputDir%\hw6_outputs\diffuse.png
 move diffuse.exr %outputDir%\hw6_outputs\diffuse.exr

 raytracer.exe ../inputs/hw6_inputs/pathTracing/inputs/cornellbox_jaroslav_path_diffuse_uniform_vs_cosine.xml >> %outputDir%\hw6_outputs\log.txt
 move diffuse_uniform.png %outputDir%\hw6_outputs\diffuse_uniform.png
 move diffuse_uniform.exr %outputDir%\hw6_outputs\diffuse_uniform.exr
 move diffuse_cosine_importance.png %outputDir%\hw6_outputs\diffuse_cosine_importance.png
 move diffuse_cosine_importance.exr %outputDir%\hw6_outputs\diffuse_cosine_importance.exr

 raytracer.exe ../inputs/hw6_inputs/pathTracing/inputs/cornellbox_jaroslav_path_glass.xml >> %outputDir%\hw6_outputs\log.txt
 move cornellbox_jaroslav_path_glass.png %outputDir%\hw6_outputs\cornellbox_jaroslav_path_glass.png
 move cornellbox_jaroslav_path_glass.exr %outputDir%\hw6_outputs\cornellbox_jaroslav_path_glass.exr

 raytracer.exe ../inputs/hw6_inputs/pathTracing/inputs/cornellbox_jaroslav_path_nee_all.xml >> %outputDir%\hw6_outputs\log.txt
 move cornellbox_jaroslav_path_nee_all.png %outputDir%\hw6_outputs\cornellbox_jaroslav_path_nee_all.png
 move cornellbox_jaroslav_path_nee_all.exr %outputDir%\hw6_outputs\cornellbox_jaroslav_path_nee_all.exr

 raytracer.exe ../inputs/hw6_inputs/pathTracing/inputs/cornellbox_jaroslav_path_nee_flat_light.xml >> %outputDir%\hw6_outputs\log.txt
 move diffuse_importance_nee.png %outputDir%\hw6_outputs\diffuse_importance_nee.png
 move diffuse_importance_nee.exr %outputDir%\hw6_outputs\diffuse_importance_nee.exr
 move diffuse_nee.png %outputDir%\hw6_outputs\diffuse_nee.png
 move diffuse_nee.exr %outputDir%\hw6_outputs\diffuse_nee.exr
 move diffuse_importance.png %outputDir%\hw6_outputs\diffuse_importance.png
 move diffuse_importance.exr %outputDir%\hw6_outputs\diffuse_importance.exr
 move diffuse.png %outputDir%\hw6_outputs\diffuse.png
 move diffuse.exr %outputDir%\hw6_outputs\diffuse.exr

 raytracer.exe ../inputs/hw6_inputs/pathTracing/inputs/cornellbox_jaroslav_path_nee_prism_light.xml >> %outputDir%\hw6_outputs\log.txt
 move diffuse_importance_nee.png %outputDir%\hw6_outputs\diffuse_importance_nee.png
 move diffuse_importance_nee.exr %outputDir%\hw6_outputs\diffuse_importance_nee.exr
 move diffuse_nee.png %outputDir%\hw6_outputs\diffuse_nee.png
 move diffuse_nee.exr %outputDir%\hw6_outputs\diffuse_nee.exr
 move diffuse_importance.png %outputDir%\hw6_outputs\diffuse_importance.png
 move diffuse_importance.exr %outputDir%\hw6_outputs\diffuse_importance.exr
 move diffuse.png %outputDir%\hw6_outputs\diffuse.png
 move diffuse.exr %outputDir%\hw6_outputs\diffuse.exr

 raytracer.exe ../inputs/hw6_inputs/pathTracing/inputs/cornellbox_jaroslav_path_nee_russian.xml >> %outputDir%\hw6_outputs\log.txt
 move diffuse_importance_nee_russian.png %outputDir%\hw6_outputs\diffuse_importance_nee_russian.png
 move diffuse_importance_nee_russian.exr %outputDir%\hw6_outputs\diffuse_importance_nee_russian.exr

 raytracer.exe ../inputs/hw6_inputs/pathTracing/inputs/cornellbox_jaroslav_path_nee_sphere_light.xml >> %outputDir%\hw6_outputs\log.txt
 move diffuse_importance_nee.png %outputDir%\hw6_outputs\diffuse_importance_nee.png
 move diffuse_importance_nee.exr %outputDir%\hw6_outputs\diffuse_importance_nee.exr
 move diffuse_nee.png %outputDir%\hw6_outputs\diffuse_nee.png
 move diffuse_nee.exr %outputDir%\hw6_outputs\diffuse_nee.exr
 move diffuse_importance.png %outputDir%\hw6_outputs\diffuse_importance.png
 move diffuse_importance.exr %outputDir%\hw6_outputs\diffuse_importance.exr
 move diffuse.png %outputDir%\hw6_outputs\diffuse.png
 move diffuse.exr %outputDir%\hw6_outputs\diffuse.exr

::::::::::::::::::::::::

 raytracer.exe ../inputs/hw6_inputs/sponza/inputs/sponza.xml >> %outputDir%\hw6_outputs\log.txt
 move sponza_direct.exr %outputDir%\hw6_outputs\sponza_direct.exr
 move sponza_direct.png %outputDir%\hw6_outputs\sponza_direct.png
 move sponza_path.exr %outputDir%\hw6_outputs\sponza_path.exr
 move sponza_path.png %outputDir%\hw6_outputs\sponza_path.png
 
::::::::::::::::::::::::

 raytracer.exe ../inputs/hw6_inputs/veach-ajar/inputs/scene.xml >> %outputDir%\hw6_outputs\log.txt
 move VeachAjar_direct.png %outputDir%\hw6_outputs\VeachAjar_direct.png
 move VeachAjar_path.png %outputDir%\hw6_outputs\VeachAjar_path.png
 move VeachAjar_path.exr %outputDir%\hw6_outputs\VeachAjar_path.exr

:project


:: :::::::::::::::::::::::: project inputs ::::::::::::::::::::::::

GOTO :project2
	del %outputDir%\project_outputs\log.txt
	raytracer.exe ../inputs/hw1_inputs/simple.xml >> %outputDir%\project_outputs\log.txt
	move simple.png %outputDir%\project_outputs\simple.png

	raytracer.exe ../inputs/hw2_inputs/metal_glass_plates.xml >> %outputDir%\project_outputs\log.txt
	move metal_glass_plates.png %outputDir%\project_outputs\metal_glass_plates.png

	raytracer.exe ../inputs/hw2_inputs/marching_dragons.xml >> %outputDir%\project_outputs\log.txt
	move marching_dragons.png %outputDir%\project_outputs\marching_dragons.png

	 raytracer.exe ../inputs/hw3_inputs/focusing_dragons.xml >> %outputDir%\project_outputs\log.txt
	 move focusing_dragons.png %outputDir%\project_outputs\focusing_dragons.png

	 raytracer.exe ../inputs/hw3_inputs/cornellbox_brushed_metal.xml >> %outputDir%\project_outputs\log.txt
	 move cornellbox_brushed_metal.png %outputDir%\project_outputs\cornellbox_brushed_metal.png


	 raytracer.exe ../inputs/hw3_inputs/cornellbox_area.xml >> %outputDir%\project_outputs\log.txt
	 move cornellbox_area.png %outputDir%\project_outputs\cornellbox_area.png

	 raytracer.exe ../inputs/hw3_inputs/cornellbox_boxes_dynamic.xml >> %outputDir%\project_outputs\log.txt
	 move cornellbox_boxes_dynamic.png %outputDir%\project_outputs\cornellbox_boxes_dynamic.png

	 raytracer.exe ../inputs/hw4_inputs/cube_waves.xml >> %outputDir%\project_outputs\log.txt
	 move cube_waves.png %outputDir%\project_outputs\cube_waves.png
	 
:project2
	 raytracer.exe ../inputs/hw4_inputs/veach_ajar/scene.xml >> %outputDir%\project_outputs\log.txt
	 move VeachAjar.png %outputDir%\project_outputs\VeachAjar.png

GOTO :project3
	raytracer.exe ../inputs/hw4_inputs/galactica_dynamic.xml >> %outputDir%\project_outputs\log.txt
	move galactica_dynamic.png %outputDir%\project_outputs\galactica_dynamic.png



	 raytracer.exe ../inputs/hw5_inputs/dragon_spot_light_msaa.xml >> %outputDir%\project_outputs\log.txt
	 move dragon_spot_light_msaa.png %outputDir%\project_outputs\dragon_spot_light_msaa.png

	 raytracer.exe ../inputs/hw5_inputs/dragon/dragon_new_ply_with_spot.xml >> %outputDir%\project_outputs\log.txt
	 move dragon_new_with_spot.png %outputDir%\project_outputs\dragon_new_with_spot.png

	:: raytracer.exe ../inputs/hw5_inputs/head_env_light.xml >> %outputDir%\project_outputs\log.txt
	:: move head_env_light.exr %outputDir%\project_outputs\head_env_light.exr
	:: move head_env_light.png %outputDir%\project_outputs\head_env_light.png

	  raytracer.exe ../inputs/hw6_inputs/brdf/inputs/killeroo_blinnphong.xml >> %outputDir%\project_outputs\log.txt
	 move killeroo_blinnphong.png %outputDir%\project_outputs\killeroo_blinnphong.png
	 move killeroo_blinnphong.exr %outputDir%\project_outputs\killeroo_blinnphong.exr
	 move killeroo_blinnphong_closeup.png %outputDir%\project_outputs\killeroo_blinnphong_closeup.png
	 move killeroo_blinnphong_closeup.exr %outputDir%\project_outputs\killeroo_blinnphong_closeup.exr




:project3