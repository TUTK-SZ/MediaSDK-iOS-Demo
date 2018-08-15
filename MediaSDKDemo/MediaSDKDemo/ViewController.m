//
//  ViewController.m
//  Demo
//
//  Created by Joe_Liu on 2018/7/27.
//  Copyright © 2018年 Joe_Liu. All rights reserved.
//

#import "ViewController.h"
#import "DeviceViewController.h"
#import "ClientViewController.h"
#import "MediaViewController.h"

#define TESTUID @"CDKA8H4CU7R4GGPGUHC1"

@interface ViewController ()

@property (nonatomic, strong)UIAlertController *alertController;
@property (nonatomic, assign)BOOL               isP2PDemo;

@end

@implementation ViewController

- (UIAlertController *)alertController {
    if (!_alertController) {
        _alertController = [UIAlertController alertControllerWithTitle:nil
                                                               message:nil
                                                        preferredStyle:UIAlertControllerStyleActionSheet];
        
        UIAlertAction *asDeviceAction = [UIAlertAction actionWithTitle:NSLocalizedString(@"as Device", nil)
                                                                 style:UIAlertActionStyleDestructive
                                                               handler:^(UIAlertAction * _Nonnull action)
        {
             if (self->_isP2PDemo) {
                 UIStoryboard *mainSB = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
                 MediaViewController *deviceVC = [mainSB instantiateViewControllerWithIdentifier:@"P2PDeviceVC"];
                 [self presentViewController:deviceVC animated:YES completion:nil];
             }else {
                 UIStoryboard *mainSB = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
                 DeviceViewController *deviceVC = [mainSB instantiateViewControllerWithIdentifier:@"DeviceViewController"];
                 [self presentViewController:deviceVC animated:YES completion:nil];
             }
        }];
        [_alertController addAction:asDeviceAction];
        
        UIAlertAction *asClientAction = [UIAlertAction actionWithTitle:NSLocalizedString(@"as Client", nil)
                                                                 style:0
                                                               handler:^(UIAlertAction * _Nonnull action)
        {
             if (self->_isP2PDemo) {
                 UIStoryboard *mainSB = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
                 MediaViewController *clientVC = [mainSB instantiateViewControllerWithIdentifier:@"P2PClientVC"];
                 [self presentViewController:clientVC animated:YES completion:nil];
             }else {
                 UIStoryboard *mainSB = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
                 ClientViewController *clientVC = [mainSB instantiateViewControllerWithIdentifier:@"ClientViewController"];
                 [self presentViewController:clientVC animated:YES completion:nil];
             }
        }];
        [_alertController addAction:asClientAction];
    }
    return _alertController;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    [[NSUserDefaults standardUserDefaults] setObject:TESTUID forKey:@"UID"]; // UID已限制
}

- (IBAction)videoCallClick:(id)sender {
    self.isP2PDemo = NO;
    [self presentAlertVC];
}

- (IBAction)mediaClick:(id)sender {
    UIStoryboard *mainSB = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
    MediaViewController *mediaVC = [mainSB instantiateViewControllerWithIdentifier:@"MediaViewController"];
    [self presentViewController:mediaVC animated:YES completion:nil];
}

- (IBAction)p2pClick:(id)sender {
    self.isP2PDemo = YES;
    [self presentAlertVC];
}

- (void)presentAlertVC {
    UIPopoverPresentationController *popover = self.alertController.popoverPresentationController;
    if (popover)
    {
        // only for iPad
        popover.sourceView = self.view;
        popover.sourceRect = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height/5*4);
        popover.permittedArrowDirections = UIPopoverArrowDirectionAny;
    }
    [self presentViewController:self.alertController animated:YES completion:nil];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
